#include "D2DataTbls.h"

#include "D2Composit.h"
#include "D2Skills.h"
#include "Units/Units.h"



//D2Common.0x6FD729C0
void __fastcall DATATBLS_LoadPlrType_ModeTxt(void* pMemPool)
{
	D2PlrModeTypeTxt* pPlrModeTypeTxt = NULL;
	D2PlrModeTypeTxt* pPlrType = NULL;
	D2PlrModeTypeTxt* pPlrMode = NULL;
	int nTypeRecords = 0;
	int nModeRecords = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_ASCII, 31, 0, NULL },
		{ "token", TXTFIELD_RAW, 0, 32, NULL },
		{ "end", 0, 0, 0, NULL }
	};

	pPlrType = (D2PlrModeTypeTxt*)DATATBLS_CompileTxt(pMemPool, "plrtype", pTbl, &nTypeRecords, sizeof(D2PlrModeTypeTxt));
	pPlrMode = (D2PlrModeTypeTxt*)DATATBLS_CompileTxt(pMemPool, "plrmode", pTbl, &nModeRecords, sizeof(D2PlrModeTypeTxt));

	sgptDataTables->pPlrModeDataTables.nPlrModeTypeTxtRecordCount = nModeRecords + nTypeRecords;

	pPlrModeTypeTxt = (D2PlrModeTypeTxt*)FOG_AllocServerMemory(NULL, sizeof(D2PlrModeTypeTxt) * (nModeRecords + nTypeRecords), __FILE__, __LINE__, 0);
	D2_ASSERT(pPlrModeTypeTxt);

	sgptDataTables->pPlrModeDataTables.pPlrModeTypeTxt = pPlrModeTypeTxt;
	sgptDataTables->pPlrModeDataTables.pPlayerType = pPlrModeTypeTxt;
	sgptDataTables->pPlrModeDataTables.pPlayerMode = pPlrModeTypeTxt + nTypeRecords;

	memcpy(sgptDataTables->pPlrModeDataTables.pPlayerType, pPlrType, sizeof(D2PlrModeTypeTxt) * nTypeRecords);
	memcpy(sgptDataTables->pPlrModeDataTables.pPlayerMode, pPlrMode, sizeof(D2PlrModeTypeTxt) * nModeRecords);

	DATATBLS_UnloadBin(pPlrType);
	DATATBLS_UnloadBin(pPlrMode);
}

//D2Common.0x6FD72B30
void __fastcall DATATBLS_LoadMonModeTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_ASCII, 31, 0, NULL },
		{ "token", TXTFIELD_RAW, 0, 32, NULL },
		{ "DT_Dir", TXTFIELD_UNKNOWN2, 0, 36, NULL },
		{ "NU_Dir", TXTFIELD_UNKNOWN2, 0, 37, NULL },
		{ "WL_Dir", TXTFIELD_UNKNOWN2, 0, 38, NULL },
		{ "GH_Dir", TXTFIELD_UNKNOWN2, 0, 39, NULL },
		{ "A1_Dir", TXTFIELD_UNKNOWN2, 0, 40, NULL },
		{ "A2_Dir", TXTFIELD_UNKNOWN2, 0, 41, NULL },
		{ "BL_Dir", TXTFIELD_UNKNOWN2, 0, 42, NULL },
		{ "SC_Dir", TXTFIELD_UNKNOWN2, 0, 43, NULL },
		{ "S1_Dir", TXTFIELD_UNKNOWN2, 0, 44, NULL },
		{ "S2_Dir", TXTFIELD_UNKNOWN2, 0, 45, NULL },
		{ "S3_Dir", TXTFIELD_UNKNOWN2, 0, 46, NULL },
		{ "S4_Dir", TXTFIELD_UNKNOWN2, 0, 47, NULL },
		{ "DD_Dir", TXTFIELD_UNKNOWN2, 0, 48, NULL },
		{ "KB_Dir", TXTFIELD_UNKNOWN2, 0, 49, NULL },
		{ "SQ_Dir", TXTFIELD_UNKNOWN2, 0, 50, NULL },
		{ "RN_Dir", TXTFIELD_UNKNOWN2, 0, 51, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	sgptDataTables->pMonModeDataTables.pMonModeTxt = (D2MonModeTxt*)DATATBLS_CompileTxt(pMemPool, "monmode", pTbl, &sgptDataTables->pMonModeDataTables.nMonModeTxtRecordCount, sizeof(D2MonModeTxt));

	sgptDataTables->pMonModeDataTables.pMonMode[0] = sgptDataTables->pMonModeDataTables.pMonModeTxt;
	sgptDataTables->pMonModeDataTables.pMonMode[1] = sgptDataTables->pMonModeDataTables.pMonModeTxt;
}

//D2Common.0x6FD72E50
void __fastcall DATATBLS_LoadObjType_ModeTxt(void* pMemPool)
{
	D2ObjModeTypeTxt* pObjModeTypeTxt = NULL;

	D2ObjModeTypeTxt* pObjType = NULL;
	D2ObjModeTypeTxt* pObjMode = NULL;
	int nTypeRecords = 0;
	int nModeRecords = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_ASCII, 31, 0, NULL },
		{ "token", TXTFIELD_RAW, 0, 32, NULL },
		{ "end", 0, 0, 0, NULL }
	};

	pObjType = (D2ObjModeTypeTxt*)DATATBLS_CompileTxt(pMemPool, "objtype", pTbl, &nTypeRecords, sizeof(D2ObjModeTypeTxt));
	pObjMode = (D2ObjModeTypeTxt*)DATATBLS_CompileTxt(pMemPool, "objmode", pTbl, &nModeRecords, sizeof(D2ObjModeTypeTxt));

	sgptDataTables->pObjModeDataTables.nObjModeTypeTxtRecordCount = nModeRecords + nTypeRecords;

	pObjModeTypeTxt = (D2ObjModeTypeTxt*)FOG_AllocServerMemory(NULL, sizeof(D2ObjModeTypeTxt) * (nModeRecords + nTypeRecords), __FILE__, __LINE__, 0);
	D2_ASSERT(pObjModeTypeTxt);

	sgptDataTables->pObjModeDataTables.pObjModeTypeTxt = pObjModeTypeTxt;
	sgptDataTables->pObjModeDataTables.pObjType = pObjModeTypeTxt;
	sgptDataTables->pObjModeDataTables.pObjMode = (D2ObjModeTypeTxt*)((char*)pObjModeTypeTxt + sizeof(D2ObjModeTypeTxt) * nTypeRecords);

	memcpy(sgptDataTables->pObjModeDataTables.pObjType, pObjType, sizeof(D2ObjModeTypeTxt) * nTypeRecords);
	memcpy(sgptDataTables->pObjModeDataTables.pObjMode, pObjMode, sizeof(D2ObjModeTypeTxt) * nModeRecords);
	DATATBLS_UnloadBin(pObjType);
	DATATBLS_UnloadBin(pObjMode);
}

//D2Common.0x6FD72FC0
void __fastcall DATATBLS_LoadCompositTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_ASCII, 31, 0, NULL },
		{ "token", TXTFIELD_RAW, 0, 32, NULL },
		{ "end", 0, 0, 0, NULL }
	};

	sgptDataTables->pCompositTxt = (D2CompositTxt*)DATATBLS_CompileTxt(pMemPool, "composit", pTbl, NULL, sizeof(D2CompositTxt));
}

//D2Common.0x6FD73040
void __fastcall DATATBLS_LoadArmTypeTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_ASCII, 31, 0, NULL },
		{ "token", TXTFIELD_RAW, 0, 32, NULL },
		{ "end", 0, 0, 0, NULL }
	};

	sgptDataTables->pArmTypeTxt = (D2ArmTypeTxt*)DATATBLS_CompileTxt(pMemPool, "armtype", pTbl, NULL, sizeof(D2ArmTypeTxt));
}

//D2Common.0x6FD730C0
void __fastcall DATATBLS_UnloadPlrMode_Type_MonMode_ObjMode_Type_Composit_ArmtypeTxt()
{
	if (sgptDataTables->pPlrModeDataTables.pPlrModeTypeTxt)
	{
		FOG_FreeServerMemory(NULL, sgptDataTables->pPlrModeDataTables.pPlrModeTypeTxt, __FILE__, __LINE__, 0);
		sgptDataTables->pPlrModeDataTables.pPlrModeTypeTxt = NULL;
	}

	if (sgptDataTables->pMonModeDataTables.pMonModeTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonModeDataTables.pMonModeTxt);
		sgptDataTables->pMonModeDataTables.pMonModeTxt = NULL;
	}

	if (sgptDataTables->pObjModeDataTables.pObjModeTypeTxt)
	{
		FOG_FreeServerMemory(NULL, sgptDataTables->pObjModeDataTables.pObjModeTypeTxt, __FILE__, __LINE__, 0);
		sgptDataTables->pObjModeDataTables.pObjModeTypeTxt = NULL;
	}

	if (sgptDataTables->pCompositTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pCompositTxt);
		sgptDataTables->pCompositTxt = NULL;
	}

	if (sgptDataTables->pArmTypeTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pArmTypeTxt);
		sgptDataTables->pArmTypeTxt = NULL;
	}
}

//D2Common.0x6FD73150 (#10643)
D2PlrModeDataTbl* __fastcall DATATBLS_GetPlrMode_TypeDataTables()
{
	return &sgptDataTables->pPlrModeDataTables;
}

//D2Common.0x6FD73160 (#10644)
D2MonModeDataTbl* __fastcall DATATBLS_GetMonModeDataTables()
{
	return &sgptDataTables->pMonModeDataTables;
}

//D2Common.0x6FD73170 (#10645)
D2ObjModeDataTbl* __fastcall DATATBLS_GetObjMode_TypeDataTables()
{
	return &sgptDataTables->pObjModeDataTables;
}

//D2Common.0x6FD73180 (#10646)
D2PlrModeTypeTxt* __stdcall DATATBLS_GetPlrModeTypeTxtRecord(int nIndex, int bGetMode)
{
	if (nIndex >= sgptDataTables->pPlrModeDataTables.nPlrModeTypeTxtRecordCount)
	{
		return NULL;
	}

	D2_ASSERT(sgptDataTables->pPlrModeDataTables.pPlrModeTypeTxt);

	if (bGetMode)
	{
		if (bGetMode == 1)
		{
			D2_ASSERT(&sgptDataTables->pPlrModeDataTables.pPlayerMode[nIndex]);
			return &sgptDataTables->pPlrModeDataTables.pPlayerMode[nIndex];
		}
		return NULL;
	}

	D2_ASSERT(&sgptDataTables->pPlrModeDataTables.pPlayerType[nIndex]);

	return &sgptDataTables->pPlrModeDataTables.pPlayerType[nIndex];
}

//D2Common.0x6FD73230 (#10647)
D2MonModeTxt* __stdcall DATATBLS_GetMonModeTxtRecord(int nIndex, int bGetMode)
{
	if (nIndex >= sgptDataTables->pMonModeDataTables.nMonModeTxtRecordCount)
	{
		return NULL;
	}

	if (bGetMode)
	{
		if (bGetMode == 1)
		{
			D2_ASSERT(&sgptDataTables->pMonModeDataTables.pMonMode[1][nIndex]);
			return &sgptDataTables->pMonModeDataTables.pMonMode[1][nIndex];
		}
		return NULL;
	}

	D2_ASSERT(&sgptDataTables->pMonModeDataTables.pMonMode[0][nIndex]);

	return &sgptDataTables->pMonModeDataTables.pMonMode[0][nIndex];
}

//D2Common.0x6FD732B0 (#10648)
D2ObjModeTypeTxt* __stdcall DATATBLS_GetObjModeTypeTxtRecord(int nIndex, int bGetMode)
{
	if (nIndex >= sgptDataTables->pObjModeDataTables.nObjModeTypeTxtRecordCount)
	{
		return NULL;
	}

	if (bGetMode)
	{
		if (bGetMode == 1)
		{
			D2_ASSERT(&sgptDataTables->pObjModeDataTables.pObjMode[nIndex]);
			return &sgptDataTables->pObjModeDataTables.pObjMode[nIndex];
		}
		return NULL;
	}

	D2_ASSERT(&sgptDataTables->pObjModeDataTables.pObjType[nIndex]);
	return &sgptDataTables->pObjModeDataTables.pObjType[nIndex];
}

//D2Common.0x6FD73330 (#10649)
D2CompositTxt* __stdcall DATATBLS_GetCompositTxtRecord(int nComposit)
{
	D2_ASSERT(&sgptDataTables->pCompositTxt[nComposit]);

	return &sgptDataTables->pCompositTxt[nComposit];
}

//D2Common.0x6FD73370 (#10650)
D2ArmTypeTxt* __stdcall DATATBLS_GetArmTypeTxtRecord(int nId)
{
	D2_ASSERT(&sgptDataTables->pArmTypeTxt[nId]);
	return &sgptDataTables->pArmTypeTxt[nId];
}
