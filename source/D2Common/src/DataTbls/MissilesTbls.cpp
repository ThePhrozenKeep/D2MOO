#include "D2DataTbls.h"
#include <Calc.h>
#include <Storm.h>

//D2Common.0x6FD62EA0
int __fastcall DATATBLS_MapMissilesTxtKeywordToNumber(char* szKey)
{
	if (!SStrCmpI(szKey, "min", 32))
	{
		return 0;
	}
	else if (!SStrCmpI(szKey, "max", 32))
	{
		return 1;
	}
	else if (!SStrCmpI(szKey, "rand", 32))
	{
		return 2;
	}
	else if (!SStrCmpI(szKey, "skill", 32))
	{
		return 3;
	}
	else if (!SStrCmpI(szKey, "miss", 32))
	{
		return 4;
	}
	else
	{
		return -1;
	}
}

//D2Common.0x6FD62F20
//TODO: Find a name
int __fastcall sub_6FD62F20(char* szText, int* a2, int a3, int nKeywordNumber)
{
	D2TxtLinkStrc* pLinker = sgptDataTables->pMissileCalcLinker;
	char szCode[4] = {};
	int nRow = 0;

	if (a3 == 1)
	{
		if (nKeywordNumber == 3)
		{
			if (sgptDataTables->iSkillCode)
			{
				nRow = FOG_GetRowFromTxt(sgptDataTables->iSkillCode, szText, 0);
				if (nRow >= 0)
				{
					*a2 = 1;
					return nRow;
				}

				pLinker = sgptDataTables->pSkillCalcLinker;
			}
		}
		else if (nKeywordNumber == 4)
		{
			if (sgptDataTables->pMissilesLinker)
			{
				nRow = FOG_GetRowFromTxt(sgptDataTables->pMissilesLinker, szText, 0);
				if (nRow >= 0)
				{
					*a2 = 1;
					return nRow;
				}
			}

			pLinker = sgptDataTables->pMissileCalcLinker;
		}
	}

	if (szText[0])
	{
		szCode[0] = szText[0];
		if (szText[1])
		{
			szCode[1] = szText[1];
			if (szText[2])
			{
				szCode[2] = szText[2];
				if (szText[3])
				{
					szCode[3] = szText[3];
				}
				else
				{
					szCode[3] = ' ';
				}
			}
			else
			{
				szCode[2] = ' ';
				szCode[3] = ' ';
			}
		}
		else
		{
			szCode[1] = ' ';
			szCode[2] = ' ';
			szCode[3] = ' ';
		}
	}
	else
	{
		szCode[0] = ' ';
		szCode[1] = ' ';
		szCode[2] = ' ';
		szCode[3] = ' ';
	}

	nRow = FOG_GetLinkIndex(pLinker, *(uint32_t*)szCode, 1);

	*a2 = 0;
	return nRow;
}

//D2Common.0x6FD630F0
void __fastcall DATATBLS_MissileCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	int nBufferSize = 0;
	FOGASTNodeStrc pBuffer[1024] = {};

	if (pRecord)
	{
		if (pSrc)
		{
			nBufferSize = DATATBLS_CompileExpression(pSrc, pBuffer, sizeof(pBuffer), DATATBLS_MapMissilesTxtKeywordToNumber, NULL, sub_6FD62F20);
			if (nBufferSize > 0)
			{
				*(uint32_t*)((char*)pRecord + nOffset) = DATATBLS_AppendMemoryBuffer((char**)&sgptDataTables->pMissCode, (int*)&sgptDataTables->nMissCodeSize, &sgptDataTables->nMissCodeSizeEx, pBuffer, nBufferSize);
			}
			else
			{
				*(uint32_t*)((char*)pRecord + nOffset) = -1;
			}
		}
		else
		{
			*(uint32_t*)((char*)pRecord + nOffset) = -1;
		}
	}
}

//D2Common.0x6FD63180
void __fastcall DATATBLS_LoadMissilesTxt(HD2ARCHIVE hArchive)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Missile", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pMissilesLinker },
		{ "LastCollide", TXTFIELD_BIT, 0, 4, NULL },
		{ "Explosion", TXTFIELD_BIT, 1, 4, NULL },
		{ "Pierce", TXTFIELD_BIT, 2, 4, NULL },
		{ "CanSlow", TXTFIELD_BIT, 3, 4, NULL },
		{ "CanDestroy", TXTFIELD_BIT, 4, 4, NULL },
		{ "NoMultiShot", TXTFIELD_BIT, 12, 4, NULL },
		{ "NoUniqueMod", TXTFIELD_BIT, 13, 4, NULL },
		{ "ClientSend", TXTFIELD_BIT, 5, 4, NULL },
		{ "GetHit", TXTFIELD_BIT, 6, 4, NULL },
		{ "SoftHit", TXTFIELD_BIT, 7, 4, NULL },
		{ "ApplyMastery", TXTFIELD_BIT, 8, 4, NULL },
		{ "ReturnFire", TXTFIELD_BIT, 9, 4, NULL },
		{ "Town", TXTFIELD_BIT, 10, 4, NULL },
		{ "SrcTown", TXTFIELD_BIT, 11, 4, NULL },
		{ "pCltDoFunc", TXTFIELD_WORD, 0, 8, NULL },
		{ "pCltHitFunc", TXTFIELD_WORD, 0, 10, NULL },
		{ "pSrvDoFunc", TXTFIELD_WORD, 0, 12, NULL },
		{ "pSrvHitFunc", TXTFIELD_WORD, 0, 14, NULL },
		{ "pSrvDmgFunc", TXTFIELD_WORD, 0, 16, NULL },
		{ "Param1", TXTFIELD_DWORD, 0, 56, NULL },
		{ "Param2", TXTFIELD_DWORD, 0, 60, NULL },
		{ "Param3", TXTFIELD_DWORD, 0, 64, NULL },
		{ "Param4", TXTFIELD_DWORD, 0, 68, NULL },
		{ "Param5", TXTFIELD_DWORD, 0, 72, NULL },
		{ "SrvCalc1", TXTFIELD_CALCTODWORD, 0, 128, DATATBLS_MissileCalcLinker },
		{ "CltParam1", TXTFIELD_DWORD, 0, 88, NULL },
		{ "CltParam2", TXTFIELD_DWORD, 0, 92, NULL },
		{ "CltParam3", TXTFIELD_DWORD, 0, 96, NULL },
		{ "CltParam4", TXTFIELD_DWORD, 0, 100, NULL },
		{ "CltParam5", TXTFIELD_DWORD, 0, 104, NULL },
		{ "CltCalc1", TXTFIELD_CALCTODWORD, 0, 132, DATATBLS_MissileCalcLinker },
		{ "sHitPar1", TXTFIELD_DWORD, 0, 76, NULL },
		{ "sHitPar2", TXTFIELD_DWORD, 0, 80, NULL },
		{ "sHitPar3", TXTFIELD_DWORD, 0, 84, NULL },
		{ "SHitCalc1", TXTFIELD_CALCTODWORD, 0, 136, DATATBLS_MissileCalcLinker },
		{ "cHitPar1", TXTFIELD_DWORD, 0, 108, NULL },
		{ "cHitPar2", TXTFIELD_DWORD, 0, 112, NULL },
		{ "cHitPar3", TXTFIELD_DWORD, 0, 116, NULL },
		{ "CHitCalc1", TXTFIELD_CALCTODWORD, 0, 140, DATATBLS_MissileCalcLinker },
		{ "dParam1", TXTFIELD_DWORD, 0, 120, NULL },
		{ "dParam2", TXTFIELD_DWORD, 0, 124, NULL },
		{ "DmgCalc1", TXTFIELD_CALCTODWORD, 0, 144, DATATBLS_MissileCalcLinker },
		{ "TravelSound", TXTFIELD_NAMETOWORD, 0, 18, &sgptDataTables->pSoundsLinker },
		{ "HitSound", TXTFIELD_NAMETOWORD, 0, 20, &sgptDataTables->pSoundsLinker },
		{ "ProgSound", TXTFIELD_NAMETOWORD, 0, 52, &sgptDataTables->pSoundsLinker },
		{ "ProgOverlay", TXTFIELD_NAMETOWORD, 0, 54, &sgptDataTables->pOverlayLinker },
		{ "ExplosionMissile", TXTFIELD_NAMETOWORD, 0, 22, &sgptDataTables->pMissilesLinker },
		{ "SubMissile1", TXTFIELD_NAMETOWORD, 0, 24, &sgptDataTables->pMissilesLinker },
		{ "SubMissile2", TXTFIELD_NAMETOWORD, 0, 26, &sgptDataTables->pMissilesLinker },
		{ "SubMissile3", TXTFIELD_NAMETOWORD, 0, 28, &sgptDataTables->pMissilesLinker },
		{ "HitSubMissile1", TXTFIELD_NAMETOWORD, 0, 36, &sgptDataTables->pMissilesLinker },
		{ "HitSubMissile2", TXTFIELD_NAMETOWORD, 0, 38, &sgptDataTables->pMissilesLinker },
		{ "HitSubMissile3", TXTFIELD_NAMETOWORD, 0, 40, &sgptDataTables->pMissilesLinker },
		{ "HitSubMissile4", TXTFIELD_NAMETOWORD, 0, 42, &sgptDataTables->pMissilesLinker },
		{ "CltSubMissile1", TXTFIELD_NAMETOWORD, 0, 30, &sgptDataTables->pMissilesLinker },
		{ "CltSubMissile2", TXTFIELD_NAMETOWORD, 0, 32, &sgptDataTables->pMissilesLinker },
		{ "CltSubMissile3", TXTFIELD_NAMETOWORD, 0, 34, &sgptDataTables->pMissilesLinker },
		{ "CltHitSubMissile1", TXTFIELD_NAMETOWORD, 0, 44, &sgptDataTables->pMissilesLinker },
		{ "CltHitSubMissile2", TXTFIELD_NAMETOWORD, 0, 46, &sgptDataTables->pMissilesLinker },
		{ "CltHitSubMissile3", TXTFIELD_NAMETOWORD, 0, 48, &sgptDataTables->pMissilesLinker },
		{ "CltHitSubMissile4", TXTFIELD_NAMETOWORD, 0, 50, &sgptDataTables->pMissilesLinker },
		{ "ResultFlags", TXTFIELD_WORD, 0, 172, NULL },
		{ "HitFlags", TXTFIELD_DWORD2, 0, 168, NULL },
		{ "HitClass", TXTFIELD_BYTE, 0, 148, NULL },
		{ "Range", TXTFIELD_WORD, 0, 150, NULL },
		{ "LevRange", TXTFIELD_WORD, 0, 152, NULL },
		{ "KnockBack", TXTFIELD_BYTE, 0, 174, NULL },
		{ "animrate", TXTFIELD_WORD, 0, 160, NULL },
		{ "xoffset", TXTFIELD_WORD, 0, 162, NULL },
		{ "yoffset", TXTFIELD_WORD, 0, 164, NULL },
		{ "zoffset", TXTFIELD_WORD, 0, 166, NULL },
		{ "MissileSkill", TXTFIELD_BIT, 15, 4, NULL },
		{ "Skill", TXTFIELD_NAMETOWORD, 0, 404, &sgptDataTables->iSkillCode },
		{ "MinDamage", TXTFIELD_DWORD, 0, 176, NULL },
		{ "MinLevDam1", TXTFIELD_DWORD, 0, 184, NULL },
		{ "MinLevDam2", TXTFIELD_DWORD, 0, 188, NULL },
		{ "MinLevDam3", TXTFIELD_DWORD, 0, 192, NULL },
		{ "MinLevDam4", TXTFIELD_DWORD, 0, 196, NULL },
		{ "MinLevDam5", TXTFIELD_DWORD, 0, 200, NULL },
		{ "MaxDamage", TXTFIELD_DWORD, 0, 180, NULL },
		{ "MaxLevDam1", TXTFIELD_DWORD, 0, 204, NULL },
		{ "MaxLevDam2", TXTFIELD_DWORD, 0, 208, NULL },
		{ "MaxLevDam3", TXTFIELD_DWORD, 0, 212, NULL },
		{ "MaxLevDam4", TXTFIELD_DWORD, 0, 216, NULL },
		{ "MaxLevDam5", TXTFIELD_DWORD, 0, 220, NULL },
		{ "DmgSymPerCalc", TXTFIELD_CALCTODWORD, 0, 224, DATATBLS_MissileCalcLinker },
		{ "EType", TXTFIELD_CODETOBYTE, 0, 228, &sgptDataTables->pElemTypesLinker },
		{ "EMin", TXTFIELD_DWORD, 0, 232, NULL },
		{ "MinELev1", TXTFIELD_DWORD, 0, 240, NULL },
		{ "MinELev2", TXTFIELD_DWORD, 0, 244, NULL },
		{ "MinELev3", TXTFIELD_DWORD, 0, 248, NULL },
		{ "MinELev4", TXTFIELD_DWORD, 0, 252, NULL },
		{ "MinELev5", TXTFIELD_DWORD, 0, 256, NULL },
		{ "EMax", TXTFIELD_DWORD, 0, 236, NULL },
		{ "MaxELev1", TXTFIELD_DWORD, 0, 260, NULL },
		{ "MaxELev2", TXTFIELD_DWORD, 0, 264, NULL },
		{ "MaxELev3", TXTFIELD_DWORD, 0, 268, NULL },
		{ "MaxELev4", TXTFIELD_DWORD, 0, 272, NULL },
		{ "MaxELev5", TXTFIELD_DWORD, 0, 276, NULL },
		{ "EDmgSymPerCalc", TXTFIELD_CALCTODWORD, 0, 280, DATATBLS_MissileCalcLinker },
		{ "ELen", TXTFIELD_DWORD, 0, 284, NULL },
		{ "ELevLen1", TXTFIELD_DWORD, 0, 288, NULL },
		{ "ELevLen2", TXTFIELD_DWORD, 0, 292, NULL },
		{ "ELevLen3", TXTFIELD_DWORD, 0, 296, NULL },
		{ "CltSrcTown", TXTFIELD_BYTE, 0, 300, NULL },
		{ "SrcDamage", TXTFIELD_BYTE, 0, 301, NULL },
		{ "SrcMissDmg", TXTFIELD_BYTE, 0, 302, NULL },
		{ "Half2HSrc", TXTFIELD_BIT, 14, 4, NULL },
		{ "Vel", TXTFIELD_BYTE, 0, 154, NULL },
		{ "VelLev", TXTFIELD_BYTE, 0, 155, NULL },
		{ "MaxVel", TXTFIELD_BYTE, 0, 156, NULL },
		{ "Accel", TXTFIELD_WORD, 0, 158, NULL },
		{ "Holy", TXTFIELD_BYTE, 0, 303, NULL },
		{ "Light", TXTFIELD_BYTE, 0, 304, NULL },
		{ "Flicker", TXTFIELD_BYTE, 0, 305, NULL },
		{ "Red", TXTFIELD_BYTE, 0, 306, NULL },
		{ "Green", TXTFIELD_BYTE, 0, 307, NULL },
		{ "Blue", TXTFIELD_BYTE, 0, 308, NULL },
		{ "InitSteps", TXTFIELD_BYTE, 0, 309, NULL },
		{ "Activate", TXTFIELD_BYTE, 0, 310, NULL },
		{ "LoopAnim", TXTFIELD_BYTE, 0, 311, NULL },
		{ "CelFile", TXTFIELD_ASCII, 64, 312, NULL },
		{ "AnimLen", TXTFIELD_BYTE, 0, 376, NULL },
		{ "RandStart", TXTFIELD_DWORD, 0, 380, NULL },
		{ "SubLoop", TXTFIELD_BYTE, 0, 384, NULL },
		{ "SubStart", TXTFIELD_BYTE, 0, 385, NULL },
		{ "SubStop", TXTFIELD_BYTE, 0, 386, NULL },
		{ "CollideType", TXTFIELD_BYTE, 0, 387, NULL },
		{ "CollideKill", TXTFIELD_BYTE, 0, 390, NULL },
		{ "CollideFriend", TXTFIELD_BYTE, 0, 391, NULL },
		{ "Collision", TXTFIELD_BYTE, 0, 388, NULL },
		{ "ClientCol", TXTFIELD_BYTE, 0, 389, NULL },
		{ "NextHit", TXTFIELD_BYTE, 0, 392, NULL },
		{ "NextDelay", TXTFIELD_BYTE, 0, 393, NULL },
		{ "Size", TXTFIELD_BYTE, 0, 394, NULL },
		{ "ToHit", TXTFIELD_BYTE, 0, 395, NULL },
		{ "AlwaysExplode", TXTFIELD_BYTE, 0, 396, NULL },
		{ "Trans", TXTFIELD_BYTE, 0, 397, NULL },
		{ "Qty", TXTFIELD_BYTE, 0, 398, NULL },
		{ "SpecialSetup", TXTFIELD_DWORD, 0, 400, NULL },
		{ "HitShift", TXTFIELD_BYTE, 0, 406, NULL },
		{ "NumDirections", TXTFIELD_BYTE, 0, 416, NULL },
		{ "AnimSpeed", TXTFIELD_BYTE, 0, 417, NULL },
		{ "LocalBlood", TXTFIELD_BYTE, 0, 418, NULL },
		{ "DamageRate", TXTFIELD_DWORD, 0, 412, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMissilesLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pMissilesTxt = (D2MissilesTxt*)DATATBLS_CompileTxt(hArchive, "missiles", pTbl, &sgptDataTables->nMissilesTxtRecordCount, sizeof(D2MissilesTxt));

	for (int i = 0; i < sgptDataTables->nMissilesTxtRecordCount; ++i)
	{
		if (sgptDataTables->pMissilesTxt[i].nCollideType > 8)
		{
			FOG_Trace("Range error in entry %d in table '%s' field '%s'.  Value must be between %d and %d.", i, "missiles", "CollideType", 0, 8);
		}

		if (sgptDataTables->pMissilesTxt[i].nCollideType < 0)
		{
			sgptDataTables->pMissilesTxt[i].nCollideType = 0;
		}
		else if (sgptDataTables->pMissilesTxt[i].nCollideType > 8)
		{
			sgptDataTables->pMissilesTxt[i].nCollideType = 8;
		}
	}

	DATATBLS_GetBinFileHandle(hArchive, "misscode", (void**)&sgptDataTables->pMissCode, (int*)&sgptDataTables->nMissCodeSize, &sgptDataTables->nMissCodeSizeEx);
}

//D2Common.0x6FD64B80
void __fastcall DATATBLS_UnloadMissilesTxt()
{
	if (sgptDataTables->pMissCode)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pMissCode);
		sgptDataTables->pMissCode = NULL;
	}
	sgptDataTables->nMissCodeSize = 0;
	sgptDataTables->nMissCodeSizeEx = 0;

	if (sgptDataTables->pMissilesTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMissilesTxt);
		sgptDataTables->pMissilesTxt = NULL;
	}

	if (sgptDataTables->pMissilesLinker)
	{
		FOG_FreeLinker(sgptDataTables->pMissilesLinker);
		sgptDataTables->pMissilesLinker = NULL;
	}
	sgptDataTables->nMissilesTxtRecordCount = 0;
}

//D2Common.0x6FD64BE0 (#10590)
int __stdcall DATATBLS_GetMissileVelocityFromMissilesTxt(int nMissileId, int nLevel)
{
	D2MissilesTxt* pMissilesTxtRecord = DATATBLS_GetMissilesTxtRecord(nMissileId);

	if (pMissilesTxtRecord)
	{
		return pMissilesTxtRecord->nVel + nLevel * pMissilesTxtRecord->nVelLev / 8;
	}

	return 0;
}

//Inlined at various places
D2MissilesTxt* __fastcall DATATBLS_GetMissilesTxtRecord(int nMissileId)
{
	if (nMissileId >= 0 && nMissileId < sgptDataTables->nMissilesTxtRecordCount)
	{
		return &sgptDataTables->pMissilesTxt[nMissileId];
	}

	return NULL;
}
