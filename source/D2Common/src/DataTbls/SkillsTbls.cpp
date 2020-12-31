#include "D2DataTbls.h"


//D2Common.0x6FD498D0
int __fastcall DATATBLS_MapSkillsTxtKeywordToNumber(char* szKey)
{
	if (!_strnicmp(szKey, "min", 32))
	{
		return 0;
	}
	else if (!_strnicmp(szKey, "max", 32))
	{
		return 1;
	}
	else if (!_strnicmp(szKey, "rand", 32))
	{
		return 2;
	}
	else if (!_strnicmp(szKey, "skill", 32))
	{
		return 3;
	}
	else if (!_strnicmp(szKey, "miss", 32))
	{
		return 4;
	}
	else if (!_strnicmp(szKey, "stat", 32))
	{
		return 5;
	}
	else if (!_strnicmp(szKey, "sklvl", 32))
	{
		return 6;
	}
	else
	{
		return -1;
	}
}

//D2Common.0x6FD49980
//TODO: Find a name
int __fastcall sub_6FD49980(int nValue)
{
	return (nValue == 6) + 2;
}

//D2Common.0x6FD49990
//TODO: Find a name
int __fastcall sub_6FD49990(char* szText, int* a2, int a3, int nKeywordNumber)
{
	D2TxtLinkStrc* pLinker = sgptDataTables->pSkillCalcLinker;
	char szCode[4] = {};
	int nRow = 0;

	if (a3 == 1)
	{
		switch (nKeywordNumber)
		{
		case 3:
		{
			if (sgptDataTables->pSkillsLinker)
			{
				nRow = FOG_GetRowFromTxt(sgptDataTables->pSkillsLinker, szText, 0);
				if (nRow >= 0)
				{
					*a2 = 1;
					return nRow;
				}
			}

			pLinker = sgptDataTables->pSkillCalcLinker;
			break;
		}
		case 4:
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
			break;
		}
		case 5:
		{
			if (sgptDataTables->pItemStatCostLinker)
			{
				nRow = FOG_GetRowFromTxt(sgptDataTables->pItemStatCostLinker, szText, 0);
				if (nRow >= 0)
				{
					*a2 = 1;
					return nRow;
				}
			}

			if (_strnicmp(szText, "base", 32))
			{
				if (_strnicmp(szText, "mod", 32))
				{
					_strnicmp(szText, "accr", 32); //Seems unused / unneeded
					*a2 = 0;
					return 0;
				}
				else
				{
					*a2 = 0;
					return 2;
				}
			}
			else
			{
				*a2 = 0;
				return 1;
			}
		}
		case 6:
		{
			if (sgptDataTables->pSkillsLinker)
			{
				nRow = FOG_GetRowFromTxt(sgptDataTables->pSkillsLinker, szText, 0);
				if (nRow >= 0)
				{
					*a2 = 1;
					return nRow;
				}
			}

			pLinker = sgptDataTables->pSkillCalcLinker;
			break;
		}
		default:
			break;
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

//D2Common.0x6FD49D20
void __fastcall DATATBLS_SkillCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	unsigned int nNewSize = 0;
	unsigned int nSizeEx = 0;
	unsigned int nSize = 0;
	int nBufferSize = 0;
	char* pCode = NULL;
	char pBuffer[1024] = {};

	if (pRecord)
	{
		if (pSrc)
		{
			nBufferSize = FOG_10254(pSrc, pBuffer, sizeof(pBuffer), DATATBLS_MapSkillsTxtKeywordToNumber, sub_6FD49980, sub_6FD49990);
			if (nBufferSize > 0)
			{
				nSize = sgptDataTables->nSkillsCodeSize;
				nSizeEx = sgptDataTables->nSkillsCodeSizeEx;
				pCode = sgptDataTables->pSkillsCode;

				if (sgptDataTables->nSkillsCodeSize + nBufferSize < sgptDataTables->nSkillsCodeSizeEx)
				{
					memcpy(&pCode[nSize], pBuffer, nBufferSize);
					sgptDataTables->nSkillsCodeSize += nBufferSize;
					*(int*)((char*)pRecord + nOffset) = nSize;
				}
				else
				{
					while (1)
					{
						nNewSize = nSizeEx + 1024;
						sgptDataTables->nSkillsCodeSizeEx = nNewSize;
						if (nNewSize >= 0x7FFFFFFF)
						{
							break;
						}

						pCode = (char*)FOG_ReallocServerMemory(NULL, pCode, nNewSize, __FILE__, __LINE__, 0);
						nSize = sgptDataTables->nSkillsCodeSize;
						nSizeEx = sgptDataTables->nSkillsCodeSizeEx;
						sgptDataTables->pSkillsCode = pCode;

						if (sgptDataTables->nSkillsCodeSize + nBufferSize < sgptDataTables->nSkillsCodeSizeEx)
						{
							memcpy(&pCode[nSize], pBuffer, nBufferSize);
							sgptDataTables->nSkillsCodeSize += nBufferSize;
							*(int*)((char*)pRecord + nOffset) = nSize;
							return;
						}
					}
					*(int*)((char*)pRecord + nOffset) = 0;
				}
			}
			else
			{
				*(int*)((char*)pRecord + nOffset) = -1;
			}
		}
		else
		{
			*(int*)((char*)pRecord + nOffset) = -1;
		}
	}
}

//D2Common.0x6FD49E40
void __fastcall DATATBLS_SkillDescCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	unsigned int nNewSize = 0;
	unsigned int nSizeEx = 0;
	unsigned int nSize = 0;
	int nBufferSize = 0;
	char* pCode = NULL;
	char pBuffer[1024] = {};

	if (pRecord)
	{
		if (pSrc)
		{
			nBufferSize = FOG_10254(pSrc, pBuffer, sizeof(pBuffer), DATATBLS_MapSkillsTxtKeywordToNumber, sub_6FD49980, sub_6FD49990);
			if (nBufferSize > 0)
			{
				nSize = sgptDataTables->nSkillDescCodeSize;
				nSizeEx = sgptDataTables->nSkillDescCodeSizeEx;
				pCode = sgptDataTables->pSkillDescCode;

				if (sgptDataTables->nSkillDescCodeSize + nBufferSize < sgptDataTables->nSkillDescCodeSizeEx)
				{
					memcpy(&pCode[nSize], pBuffer, nBufferSize);
					sgptDataTables->nSkillDescCodeSize += nBufferSize;
					*(int*)((char*)pRecord + nOffset) = nSize;
				}
				else
				{
					while (1)
					{
						nNewSize = nSizeEx + 1024;
						sgptDataTables->nSkillDescCodeSizeEx = nNewSize;
						if (nNewSize >= 0x7FFFFFFF)
						{
							break;
						}

						pCode = (char*)FOG_ReallocServerMemory(NULL, pCode, nNewSize, __FILE__, __LINE__, 0);
						nSize = sgptDataTables->nSkillDescCodeSize;
						nSizeEx = sgptDataTables->nSkillDescCodeSizeEx;
						sgptDataTables->pSkillDescCode = pCode;

						if (sgptDataTables->nSkillDescCodeSize + nBufferSize < sgptDataTables->nSkillDescCodeSizeEx)
						{
							memcpy(&pCode[nSize], pBuffer, nBufferSize);
							sgptDataTables->nSkillDescCodeSize += nBufferSize;
							*(int*)((char*)pRecord + nOffset) = nSize;
							return;
						}
					}
					*(int*)((char*)pRecord + nOffset) = 0;
				}
			}
			else
			{
				*(int*)((char*)pRecord + nOffset) = -1;
			}
		}
		else
		{
			*(int*)((char*)pRecord + nOffset) = -1;
		}
	}
}

//D2Common.0x6FD49F60
void __fastcall DATATBLS_LoadSkills_SkillDescTxt(void* pMemPool)
{
	D2PetTypeTxt* pPetTypeTxtRecord = NULL;
	D2TxtLinkStrc* pSkillDescLinker = NULL;
	D2TxtLinkStrc* pMonStatsLinker = NULL;
	D2TxtLinkStrc* pRangeLinker = NULL;
	FILE* pSkillDescCodeBin = NULL;
	FILE* pSkillsCodeBin = NULL;
	void* pTmpSkillDescTxt = NULL;
	void* pTmpMonStatsTxt = NULL;
	int nHighestClassSkillCount = 0;
	int nSize = 0;
	uint8_t nPetType = 0;
	uint8_t nClass = 0;
	char szFileName[260] = {};

	D2BinFieldStrc pTmpMonStatsTbl[] =
	{
		{ "Id", TXTFIELD_NAMETOINDEX, 0, 0, &pMonStatsLinker },
		{ "end", 0, 0, 0, NULL },
	};

	D2BinFieldStrc pTmpSkillDescTbl[] =
	{
		{ "skilldesc", TXTFIELD_NAMETOINDEX, 0, 0, &pSkillDescLinker },
		{ "end", 0, 0, 0, NULL },
	};

	D2BinFieldStrc pSkillTbl[] =
	{
		{ "skill", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pSkillsLinker },
		{ "charclass", TXTFIELD_CODETOBYTE, 0, 12, &sgptDataTables->pPlayerClassLinker },
		{ "skilldesc", TXTFIELD_NAMETOWORD, 0, 404, &pSkillDescLinker },
		{ "srvstfunc", TXTFIELD_WORD, 0, 44, NULL },
		{ "srvdofunc", TXTFIELD_WORD, 0, 46, NULL },
		{ "srvprgfunc1", TXTFIELD_WORD, 0, 48, NULL },
		{ "srvprgfunc2", TXTFIELD_WORD, 0, 50, NULL },
		{ "srvprgfunc3", TXTFIELD_WORD, 0, 52, NULL },
		{ "prgcalc1", TXTFIELD_CALCTODWORD, 0, 56, DATATBLS_SkillCalcLinker },
		{ "prgcalc2", TXTFIELD_CALCTODWORD, 0, 60, DATATBLS_SkillCalcLinker },
		{ "prgcalc3", TXTFIELD_CALCTODWORD, 0, 64, DATATBLS_SkillCalcLinker },
		{ "prgdam", TXTFIELD_BYTE, 0, 68, NULL },
		{ "srvmissile", TXTFIELD_NAMETOWORD, 0, 70, &sgptDataTables->pMissilesLinker },
		{ "srvmissilea", TXTFIELD_NAMETOWORD, 0, 72, &sgptDataTables->pMissilesLinker },
		{ "srvmissileb", TXTFIELD_NAMETOWORD, 0, 74, &sgptDataTables->pMissilesLinker },
		{ "srvmissilec", TXTFIELD_NAMETOWORD, 0, 76, &sgptDataTables->pMissilesLinker },
		{ "srvoverlay", TXTFIELD_NAMETOWORD, 0, 78, &sgptDataTables->pOverlayLinker },
		{ "lob", TXTFIELD_BIT, 1, 4, NULL },
		{ "decquant", TXTFIELD_BIT, 0, 4, NULL },
		{ "immediate", TXTFIELD_BIT, 15, 4, NULL },
		{ "aurafilter", TXTFIELD_DWORD, 0, 80, NULL },
		{ "aurastate", TXTFIELD_NAMETOWORD, 0, 128, &sgptDataTables->pStatesLinker },
		{ "auratargetstate", TXTFIELD_NAMETOWORD, 0, 130, &sgptDataTables->pStatesLinker },
		{ "auralencalc", TXTFIELD_CALCTODWORD, 0, 96, DATATBLS_SkillCalcLinker },
		{ "aurarangecalc", TXTFIELD_CALCTODWORD, 0, 100, DATATBLS_SkillCalcLinker },
		{ "aurastat1", TXTFIELD_NAMETOWORD, 0, 84, &sgptDataTables->pItemStatCostLinker },
		{ "aurastatcalc1", TXTFIELD_CALCTODWORD, 0, 104, DATATBLS_SkillCalcLinker },
		{ "aurastat2", TXTFIELD_NAMETOWORD, 0, 86, &sgptDataTables->pItemStatCostLinker },
		{ "aurastatcalc2", TXTFIELD_CALCTODWORD, 0, 108, DATATBLS_SkillCalcLinker },
		{ "aurastat3", TXTFIELD_NAMETOWORD, 0, 88, &sgptDataTables->pItemStatCostLinker },
		{ "aurastatcalc3", TXTFIELD_CALCTODWORD, 0, 112, DATATBLS_SkillCalcLinker },
		{ "aurastat4", TXTFIELD_NAMETOWORD, 0, 90, &sgptDataTables->pItemStatCostLinker },
		{ "aurastatcalc4", TXTFIELD_CALCTODWORD, 0, 116, DATATBLS_SkillCalcLinker },
		{ "aurastat5", TXTFIELD_NAMETOWORD, 0, 92, &sgptDataTables->pItemStatCostLinker },
		{ "aurastatcalc5", TXTFIELD_CALCTODWORD, 0, 120, DATATBLS_SkillCalcLinker },
		{ "aurastat6", TXTFIELD_NAMETOWORD, 0, 94, &sgptDataTables->pItemStatCostLinker },
		{ "aurastatcalc6", TXTFIELD_CALCTODWORD, 0, 124, DATATBLS_SkillCalcLinker },
		{ "auraevent1", TXTFIELD_NAMETOWORD, 0, 132, &sgptDataTables->pEventsLinker },
		{ "auraeventfunc1", TXTFIELD_WORD, 0, 138, NULL },
		{ "auraevent2", TXTFIELD_NAMETOWORD, 0, 134, &sgptDataTables->pEventsLinker },
		{ "auraeventfunc2", TXTFIELD_WORD, 0, 140, NULL },
		{ "auraevent3", TXTFIELD_NAMETOWORD, 0, 136, &sgptDataTables->pEventsLinker },
		{ "auraeventfunc3", TXTFIELD_WORD, 0, 142, NULL },
		{ "auratgtevent", TXTFIELD_NAMETOWORD, 0, 144, &sgptDataTables->pEventsLinker },
		{ "auratgteventfunc", TXTFIELD_WORD, 0, 146, NULL },
		{ "passivestate", TXTFIELD_NAMETOWORD, 0, 148, &sgptDataTables->pStatesLinker },
		{ "passiveitype", TXTFIELD_CODETOWORD, 0, 150, &sgptDataTables->pItemTypesLinker },
		{ "passivestat1", TXTFIELD_NAMETOWORD, 0, 152, &sgptDataTables->pItemStatCostLinker },
		{ "passivecalc1", TXTFIELD_CALCTODWORD, 0, 164, DATATBLS_SkillCalcLinker },
		{ "passivestat2", TXTFIELD_NAMETOWORD, 0, 154, &sgptDataTables->pItemStatCostLinker },
		{ "passivecalc2", TXTFIELD_CALCTODWORD, 0, 168, DATATBLS_SkillCalcLinker },
		{ "passivestat3", TXTFIELD_NAMETOWORD, 0, 156, &sgptDataTables->pItemStatCostLinker },
		{ "passivecalc3", TXTFIELD_CALCTODWORD, 0, 172, DATATBLS_SkillCalcLinker },
		{ "passivestat4", TXTFIELD_NAMETOWORD, 0, 158, &sgptDataTables->pItemStatCostLinker },
		{ "passivecalc4", TXTFIELD_CALCTODWORD, 0, 176, DATATBLS_SkillCalcLinker },
		{ "passivestat5", TXTFIELD_NAMETOWORD, 0, 160, &sgptDataTables->pItemStatCostLinker },
		{ "passivecalc5", TXTFIELD_CALCTODWORD, 0, 180, DATATBLS_SkillCalcLinker },
		{ "passiveevent", TXTFIELD_NAMETOWORD, 0, 184, &sgptDataTables->pEventsLinker },
		{ "passiveeventfunc", TXTFIELD_WORD, 0, 186, NULL },
		{ "summon", TXTFIELD_NAMETOWORD, 0, 188, &pMonStatsLinker },
		{ "pettype", TXTFIELD_NAMETOWORD2, 0, 190, &sgptDataTables->pPetTypeLinker },
		{ "summode", TXTFIELD_CODETOBYTE, 0, 191, &sgptDataTables->pMonModeLinker },
		{ "petmax", TXTFIELD_CALCTODWORD, 0, 192, DATATBLS_SkillCalcLinker },
		{ "sumskill1", TXTFIELD_NAMETOWORD, 0, 196, &sgptDataTables->pSkillsLinker },
		{ "sumsk1calc", TXTFIELD_CALCTODWORD, 0, 208, DATATBLS_SkillCalcLinker },
		{ "sumskill2", TXTFIELD_NAMETOWORD, 0, 198, &sgptDataTables->pSkillsLinker },
		{ "sumsk2calc", TXTFIELD_CALCTODWORD, 0, 212, DATATBLS_SkillCalcLinker },
		{ "sumskill3", TXTFIELD_NAMETOWORD, 0, 200, &sgptDataTables->pSkillsLinker },
		{ "sumsk3calc", TXTFIELD_CALCTODWORD, 0, 216, DATATBLS_SkillCalcLinker },
		{ "sumskill4", TXTFIELD_NAMETOWORD, 0, 202, &sgptDataTables->pSkillsLinker },
		{ "sumsk4calc", TXTFIELD_CALCTODWORD, 0, 220, DATATBLS_SkillCalcLinker },
		{ "sumskill5", TXTFIELD_NAMETOWORD, 0, 204, &sgptDataTables->pSkillsLinker },
		{ "sumsk5calc", TXTFIELD_CALCTODWORD, 0, 224, DATATBLS_SkillCalcLinker },
		{ "sumumod", TXTFIELD_WORD, 0, 228, NULL },
		{ "sumoverlay", TXTFIELD_NAMETOWORD, 0, 230, &sgptDataTables->pOverlayLinker },
		{ "cltmissile", TXTFIELD_NAMETOWORD, 0, 232, &sgptDataTables->pMissilesLinker },
		{ "cltmissilea", TXTFIELD_NAMETOWORD, 0, 234, &sgptDataTables->pMissilesLinker },
		{ "cltmissileb", TXTFIELD_NAMETOWORD, 0, 236, &sgptDataTables->pMissilesLinker },
		{ "cltmissilec", TXTFIELD_NAMETOWORD, 0, 238, &sgptDataTables->pMissilesLinker },
		{ "cltmissiled", TXTFIELD_NAMETOWORD, 0, 240, &sgptDataTables->pMissilesLinker },
		{ "cltstfunc", TXTFIELD_WORD, 0, 242, NULL },
		{ "cltdofunc", TXTFIELD_WORD, 0, 244, NULL },
		{ "cltprgfunc1", TXTFIELD_WORD, 0, 246, NULL },
		{ "cltprgfunc2", TXTFIELD_WORD, 0, 248, NULL },
		{ "cltprgfunc3", TXTFIELD_WORD, 0, 250, NULL },
		{ "stsuccessonly", TXTFIELD_BIT, 12, 4, NULL },
		{ "stsound", TXTFIELD_NAMETOWORD, 0, 252, &sgptDataTables->pSoundsLinker },
		{ "stsoundclass", TXTFIELD_NAMETOWORD, 0, 254, &sgptDataTables->pSoundsLinker },
		{ "stsounddelay", TXTFIELD_BIT, 13, 4, NULL },
		{ "weaponsnd", TXTFIELD_BIT, 14, 4, NULL },
		{ "dosound", TXTFIELD_NAMETOWORD, 0, 256, &sgptDataTables->pSoundsLinker },
		{ "dosound a", TXTFIELD_NAMETOWORD, 0, 258, &sgptDataTables->pSoundsLinker },
		{ "dosound b", TXTFIELD_NAMETOWORD, 0, 260, &sgptDataTables->pSoundsLinker },
		{ "tgtsound", TXTFIELD_NAMETOWORD, 0, 266, &sgptDataTables->pSoundsLinker },
		{ "tgtoverlay", TXTFIELD_NAMETOWORD, 0, 264, &sgptDataTables->pOverlayLinker },
		{ "castoverlay", TXTFIELD_NAMETOWORD, 0, 262, &sgptDataTables->pOverlayLinker },
		{ "cltoverlaya", TXTFIELD_NAMETOWORD, 0, 272, &sgptDataTables->pOverlayLinker },
		{ "cltoverlayb", TXTFIELD_NAMETOWORD, 0, 274, &sgptDataTables->pOverlayLinker },
		{ "prgsound", TXTFIELD_NAMETOWORD, 0, 270, &sgptDataTables->pSoundsLinker },
		{ "prgoverlay", TXTFIELD_NAMETOWORD, 0, 268, &sgptDataTables->pOverlayLinker },
		{ "cltcalc1", TXTFIELD_CALCTODWORD, 0, 276, DATATBLS_SkillCalcLinker },
		{ "cltcalc2", TXTFIELD_CALCTODWORD, 0, 280, DATATBLS_SkillCalcLinker },
		{ "cltcalc3", TXTFIELD_CALCTODWORD, 0, 284, DATATBLS_SkillCalcLinker },
		{ "warp", TXTFIELD_BIT, 38, 4, NULL },
		{ "anim", TXTFIELD_CODETOBYTE, 0, 16, &sgptDataTables->pPlrModeLinker },
		{ "seqtrans", TXTFIELD_CODETOBYTE, 0, 18, &sgptDataTables->pPlrModeLinker },
		{ "monanim", TXTFIELD_CODETOBYTE, 0, 17, &sgptDataTables->pMonModeLinker },
		{ "seqnum", TXTFIELD_BYTE, 0, 19, NULL },
		{ "seqinput", TXTFIELD_BYTE, 0, 22, NULL },
		{ "range", TXTFIELD_CODETOBYTE, 0, TXTFIELD_NAMETOWORD, &pRangeLinker },
		{ "weapsel", TXTFIELD_BYTE, 0, 360, NULL },
		{ "itypea1", TXTFIELD_CODETOWORD, 0, 24, &sgptDataTables->pItemTypesLinker },
		{ "itypea2", TXTFIELD_CODETOWORD, 0, 26, &sgptDataTables->pItemTypesLinker },
		{ "itypea3", TXTFIELD_CODETOWORD, 0, 28, &sgptDataTables->pItemTypesLinker },
		{ "etypea1", TXTFIELD_CODETOWORD, 0, 36, &sgptDataTables->pItemTypesLinker },
		{ "etypea2", TXTFIELD_CODETOWORD, 0, 38, &sgptDataTables->pItemTypesLinker },
		{ "itypeb1", TXTFIELD_CODETOWORD, 0, 30, &sgptDataTables->pItemTypesLinker },
		{ "itypeb2", TXTFIELD_CODETOWORD, 0, 32, &sgptDataTables->pItemTypesLinker },
		{ "itypeb3", TXTFIELD_CODETOWORD, 0, 34, &sgptDataTables->pItemTypesLinker },
		{ "etypeb1", TXTFIELD_CODETOWORD, 0, 40, &sgptDataTables->pItemTypesLinker },
		{ "etypeb2", TXTFIELD_CODETOWORD, 0, 42, &sgptDataTables->pItemTypesLinker },
		{ "maxlvl", TXTFIELD_WORD, 0, 300, NULL },
		{ "progressive", TXTFIELD_BIT, 2, 4, NULL },
		{ "finishing", TXTFIELD_BIT, 3, 4, NULL },
		{ "prgstack", TXTFIELD_BIT, 7, 4, NULL },
		{ "InTown", TXTFIELD_BIT, 8, 4, NULL },
		{ "ResultFlags", TXTFIELD_WORD, 0, 302, NULL },
		{ "HitFlags", TXTFIELD_DWORD2, 0, 304, NULL },
		{ "HitClass", TXTFIELD_DWORD2, 0, 308, NULL },
		{ "Kick", TXTFIELD_BIT, 9, 4, NULL },
		{ "passive", TXTFIELD_BIT, 4, 4, NULL },
		{ "aura", TXTFIELD_BIT, 5, 4, NULL },
		{ "periodic", TXTFIELD_BIT, 6, 4, NULL },
		{ "perdelay", TXTFIELD_CALCTODWORD, 0, 296, DATATBLS_SkillCalcLinker },
		{ "general", TXTFIELD_BIT, 35, 4, NULL },
		{ "scroll", TXTFIELD_BIT, 36, 4, NULL },
		{ "ItemEffect", TXTFIELD_WORD, 0, 362, NULL },
		{ "ItemCltEffect", TXTFIELD_WORD, 0, 364, NULL },
		{ "ItemTgtDo", TXTFIELD_BIT, 29, 4, NULL },
		{ "ItemCheckStart", TXTFIELD_BIT, 33, 4, NULL },
		{ "ItemCltCheckStart", TXTFIELD_BIT, 34, 4, NULL },
		{ "ItemTarget", TXTFIELD_BYTE, 0, 288, NULL },
		{ "ItemCastSound", TXTFIELD_NAMETOWORD, 0, 290, &sgptDataTables->pSoundsLinker },
		{ "ItemCastOverlay", TXTFIELD_NAMETOWORD, 0, 292, &sgptDataTables->pOverlayLinker },
		{ "InGame", TXTFIELD_BIT, 10, 4, NULL },
		{ "lineofsight", TXTFIELD_BYTE, 0, 399, NULL },
		{ "attackrank", TXTFIELD_BYTE, 0, 398, NULL },
		{ "noammo", TXTFIELD_BIT, 16, 4, NULL },
		{ "enhanceable", TXTFIELD_BIT, 17, 4, NULL },
		{ "durability", TXTFIELD_BIT, 18, 4, NULL },
		{ "UseAttackRate", TXTFIELD_BIT, 19, 4, NULL },
		{ "SelectProc", TXTFIELD_BYTE, 0, 21, NULL },
		{ "TargetableOnly", TXTFIELD_BIT, 20, 4, NULL },
		{ "SearchEnemyXY", TXTFIELD_BIT, 21, 4, NULL },
		{ "SearchEnemyNear", TXTFIELD_BIT, 22, 4, NULL },
		{ "SearchOpenXY", TXTFIELD_BIT, 23, 4, NULL },
		{ "TargetCorpse", TXTFIELD_BIT, 24, 4, NULL },
		{ "TargetPet", TXTFIELD_BIT, 25, 4, NULL },
		{ "TargetAlly", TXTFIELD_BIT, 26, 4, NULL },
		{ "TargetItem", TXTFIELD_BIT, 27, 4, NULL },
		{ "AttackNoMana", TXTFIELD_BIT, 28, 4, NULL },
		{ "leftskill", TXTFIELD_BIT, 30, 4, NULL },
		{ "interrupt", TXTFIELD_BIT, 31, 4, NULL },
		{ "TgtPlaceCheck", TXTFIELD_BIT, 32, 4, NULL },
		{ "repeat", TXTFIELD_BIT, 11, 4, NULL },
		{ "skpoints", TXTFIELD_CALCTODWORD, 0, 368, DATATBLS_SkillCalcLinker },
		{ "reqlevel", TXTFIELD_WORD, 0, 372, NULL },
		{ "reqstr", TXTFIELD_WORD, 0, 374, NULL },
		{ "reqdex", TXTFIELD_WORD, 0, 376, NULL },
		{ "reqint", TXTFIELD_WORD, 0, 378, NULL },
		{ "reqvit", TXTFIELD_WORD, 0, 380, NULL },
		{ "reqskill1", TXTFIELD_NAMETOWORD, 0, 382, &sgptDataTables->pSkillsLinker },
		{ "reqskill2", TXTFIELD_NAMETOWORD, 0, 384, &sgptDataTables->pSkillsLinker },
		{ "reqskill3", TXTFIELD_NAMETOWORD, 0, 386, &sgptDataTables->pSkillsLinker },
		{ "delay", TXTFIELD_CALCTODWORD, 0, 400, DATATBLS_SkillCalcLinker },
		{ "usemanaondo", TXTFIELD_BIT, 37, 4, NULL },
		{ "startmana", TXTFIELD_WORD, 0, 388, NULL },
		{ "minmana", TXTFIELD_WORD, 0, 390, NULL },
		{ "manashift", TXTFIELD_WORD, 0, 392, NULL },
		{ "mana", TXTFIELD_WORD, 0, 394, NULL },
		{ "lvlmana", TXTFIELD_WORD, 0, 396, NULL },
		{ "Param1", TXTFIELD_DWORD, 0, 328, NULL },
		{ "Param2", TXTFIELD_DWORD, 0, 332, NULL },
		{ "Param3", TXTFIELD_DWORD, 0, 336, NULL },
		{ "Param4", TXTFIELD_DWORD, 0, 340, NULL },
		{ "Param5", TXTFIELD_DWORD, 0, 344, NULL },
		{ "Param6", TXTFIELD_DWORD, 0, 348, NULL },
		{ "Param7", TXTFIELD_DWORD, 0, 352, NULL },
		{ "Param8", TXTFIELD_DWORD, 0, 356, NULL },
		{ "calc1", TXTFIELD_CALCTODWORD, 0, 312, DATATBLS_SkillCalcLinker },
		{ "calc2", TXTFIELD_CALCTODWORD, 0, 316, DATATBLS_SkillCalcLinker },
		{ "calc3", TXTFIELD_CALCTODWORD, 0, 320, DATATBLS_SkillCalcLinker },
		{ "calc4", TXTFIELD_CALCTODWORD, 0, 324, DATATBLS_SkillCalcLinker },
		{ "ToHit", TXTFIELD_DWORD, 0, 408, NULL },
		{ "LevToHit", TXTFIELD_DWORD, 0, 412, NULL },
		{ "ToHitCalc", TXTFIELD_CALCTODWORD, 0, 416, DATATBLS_SkillCalcLinker },
		{ "HitShift", TXTFIELD_BYTE, 0, 420, NULL },
		{ "SrcDam", TXTFIELD_BYTE, 0, 421, NULL },
		{ "MinDam", TXTFIELD_DWORD, 0, 424, NULL },
		{ "MinLevDam1", TXTFIELD_DWORD, 0, 432, NULL },
		{ "MinLevDam2", TXTFIELD_DWORD, 0, 436, NULL },
		{ "MinLevDam3", TXTFIELD_DWORD, 0, 440, NULL },
		{ "MinLevDam4", TXTFIELD_DWORD, 0, 444, NULL },
		{ "MinLevDam5", TXTFIELD_DWORD, 0, 448, NULL },
		{ "MaxDam", TXTFIELD_DWORD, 0, 428, NULL },
		{ "MaxLevDam1", TXTFIELD_DWORD, 0, 452, NULL },
		{ "MaxLevDam2", TXTFIELD_DWORD, 0, 456, NULL },
		{ "MaxLevDam3", TXTFIELD_DWORD, 0, 460, NULL },
		{ "MaxLevDam4", TXTFIELD_DWORD, 0, 464, NULL },
		{ "MaxLevDam5", TXTFIELD_DWORD, 0, 468, NULL },
		{ "DmgSymPerCalc", TXTFIELD_CALCTODWORD, 0, 472, DATATBLS_SkillCalcLinker },
		{ "EType", TXTFIELD_CODETOBYTE, 0, 476, &sgptDataTables->pElemTypesLinker },
		{ "EMin", TXTFIELD_DWORD, 0, 480, NULL },
		{ "EMinLev1", TXTFIELD_DWORD, 0, 488, NULL },
		{ "EMinLev2", TXTFIELD_DWORD, 0, 492, NULL },
		{ "EMinLev3", TXTFIELD_DWORD, 0, 496, NULL },
		{ "EMinLev4", TXTFIELD_DWORD, 0, 500, NULL },
		{ "EMinLev5", TXTFIELD_DWORD, 0, 504, NULL },
		{ "EMax", TXTFIELD_DWORD, 0, 484, NULL },
		{ "EMaxLev1", TXTFIELD_DWORD, 0, 508, NULL },
		{ "EMaxLev2", TXTFIELD_DWORD, 0, 512, NULL },
		{ "EMaxLev3", TXTFIELD_DWORD, 0, 516, NULL },
		{ "EMaxLev4", TXTFIELD_DWORD, 0, 520, NULL },
		{ "EMaxLev5", TXTFIELD_DWORD, 0, 524, NULL },
		{ "EDmgSymPerCalc", TXTFIELD_CALCTODWORD, 0, 528, DATATBLS_SkillCalcLinker },
		{ "ELen", TXTFIELD_DWORD, 0, 532, NULL },
		{ "ELevLen1", TXTFIELD_DWORD, 0, 536, NULL },
		{ "ELevLen2", TXTFIELD_DWORD, 0, 540, NULL },
		{ "ELevLen3", TXTFIELD_DWORD, 0, 544, NULL },
		{ "ELenSymPerCalc", TXTFIELD_CALCTODWORD, 0, 548, DATATBLS_SkillCalcLinker },
		{ "restrict", TXTFIELD_BYTE, 0, 552, NULL },
		{ "state1", TXTFIELD_NAMETOWORD, 0, 554, &sgptDataTables->pStatesLinker },
		{ "state2", TXTFIELD_NAMETOWORD, 0, 556, &sgptDataTables->pStatesLinker },
		{ "state3", TXTFIELD_NAMETOWORD, 0, 558, &sgptDataTables->pStatesLinker },
		{ "aitype", TXTFIELD_BYTE, 0, 560, NULL },
		{ "aibonus", TXTFIELD_WORD, 0, 562, NULL },
		{ "cost mult", TXTFIELD_DWORD, 0, 564, NULL },
		{ "cost add", TXTFIELD_DWORD, 0, 568, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	D2BinFieldStrc pSkillDescTbl[] =
	{
		{ "skilldesc", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pSkillDescLinker },
		{ "skillpage", TXTFIELD_BYTE, 0, 2, NULL },
		{ "skillrow", TXTFIELD_BYTE, 0, TXTFIELD_WORD, NULL },
		{ "skillcolumn", TXTFIELD_BYTE, 0, 4, NULL },
		{ "ListRow", TXTFIELD_BYTE, 0, 5, NULL },
		{ "ListPool", TXTFIELD_BYTE, 0, 6, NULL },
		{ "iconcel", TXTFIELD_BYTE, 0, 7, NULL },
		{ "str name", TXTFIELD_KEYTOWORD, 0, 8, DATATBLS_GetStringIdFromReferenceString },
		{ "str short", TXTFIELD_KEYTOWORD, 0, 10, DATATBLS_GetStringIdFromReferenceString },
		{ "str long", TXTFIELD_KEYTOWORD, 0, 12, DATATBLS_GetStringIdFromReferenceString },
		{ "str alt", TXTFIELD_KEYTOWORD, 0, 14, DATATBLS_GetStringIdFromReferenceString },
		{ "str mana", TXTFIELD_KEYTOWORD, 0, 16, DATATBLS_GetStringIdFromReferenceString },
		{ "descdam", TXTFIELD_WORD, 0, 18, NULL },
		{ "descatt", TXTFIELD_WORD, 0, TXTFIELD_NAMETOWORD, NULL },
		{ "ddam calc1", TXTFIELD_CALCTODWORD, 0, 24, DATATBLS_SkillDescCalcLinker },
		{ "ddam calc2", TXTFIELD_CALCTODWORD, 0, 28, DATATBLS_SkillDescCalcLinker },
		{ "p1dmelem", TXTFIELD_CODETOBYTE, 0, 32, &sgptDataTables->pElemTypesLinker },
		{ "p1dmmin", TXTFIELD_CALCTODWORD, 0, 36, DATATBLS_SkillDescCalcLinker },
		{ "p1dmmax", TXTFIELD_CALCTODWORD, 0, 48, DATATBLS_SkillDescCalcLinker },
		{ "p2dmelem", TXTFIELD_CODETOBYTE, 0, 33, &sgptDataTables->pElemTypesLinker },
		{ "p2dmmin", TXTFIELD_CALCTODWORD, 0, 40, DATATBLS_SkillDescCalcLinker },
		{ "p2dmmax", TXTFIELD_CALCTODWORD, 0, 52, DATATBLS_SkillDescCalcLinker },
		{ "p3dmelem", TXTFIELD_CODETOBYTE, 0, 34, &sgptDataTables->pElemTypesLinker },
		{ "p3dmmin", TXTFIELD_CALCTODWORD, 0, 44, DATATBLS_SkillDescCalcLinker },
		{ "p3dmmax", TXTFIELD_CALCTODWORD, 0, 56, DATATBLS_SkillDescCalcLinker },
		{ "descmissile1", TXTFIELD_NAMETOWORD, 0, 60, &sgptDataTables->pMissilesLinker },
		{ "descmissile2", TXTFIELD_NAMETOWORD, 0, 62, &sgptDataTables->pMissilesLinker },
		{ "descmissile3", TXTFIELD_NAMETOWORD, 0, 64, &sgptDataTables->pMissilesLinker },
		{ "descline1", TXTFIELD_BYTE, 0, 66, NULL },
		{ "desctexta1", TXTFIELD_KEYTOWORD, 0, 84, DATATBLS_GetStringIdFromReferenceString },
		{ "desctextb1", TXTFIELD_KEYTOWORD, 0, 118, DATATBLS_GetStringIdFromReferenceString },
		{ "desccalca1", TXTFIELD_CALCTODWORD, 0, 152, DATATBLS_SkillDescCalcLinker },
		{ "desccalcb1", TXTFIELD_CALCTODWORD, 0, 220, DATATBLS_SkillDescCalcLinker },
		{ "descline2", TXTFIELD_BYTE, 0, 67, NULL },
		{ "desctexta2", TXTFIELD_KEYTOWORD, 0, 86, DATATBLS_GetStringIdFromReferenceString },
		{ "desctextb2", TXTFIELD_KEYTOWORD, 0, 120, DATATBLS_GetStringIdFromReferenceString },
		{ "desccalca2", TXTFIELD_CALCTODWORD, 0, 156, DATATBLS_SkillDescCalcLinker },
		{ "desccalcb2", TXTFIELD_CALCTODWORD, 0, 224, DATATBLS_SkillDescCalcLinker },
		{ "descline3", TXTFIELD_BYTE, 0, 68, NULL },
		{ "desctexta3", TXTFIELD_KEYTOWORD, 0, 88, DATATBLS_GetStringIdFromReferenceString },
		{ "desctextb3", TXTFIELD_KEYTOWORD, 0, 122, DATATBLS_GetStringIdFromReferenceString },
		{ "desccalca3", TXTFIELD_CALCTODWORD, 0, 160, DATATBLS_SkillDescCalcLinker },
		{ "desccalcb3", TXTFIELD_CALCTODWORD, 0, 228, DATATBLS_SkillDescCalcLinker },
		{ "descline4", TXTFIELD_BYTE, 0, 69, NULL },
		{ "desctexta4", TXTFIELD_KEYTOWORD, 0, 90, DATATBLS_GetStringIdFromReferenceString },
		{ "desctextb4", TXTFIELD_KEYTOWORD, 0, 124, DATATBLS_GetStringIdFromReferenceString },
		{ "desccalca4", TXTFIELD_CALCTODWORD, 0, 164, DATATBLS_SkillDescCalcLinker },
		{ "desccalcb4", TXTFIELD_CALCTODWORD, 0, 232, DATATBLS_SkillDescCalcLinker },
		{ "descline5", TXTFIELD_BYTE, 0, 70, NULL },
		{ "desctexta5", TXTFIELD_KEYTOWORD, 0, 92, DATATBLS_GetStringIdFromReferenceString },
		{ "desctextb5", TXTFIELD_KEYTOWORD, 0, 126, DATATBLS_GetStringIdFromReferenceString },
		{ "desccalca5", TXTFIELD_CALCTODWORD, 0, 168, DATATBLS_SkillDescCalcLinker },
		{ "desccalcb5", TXTFIELD_CALCTODWORD, 0, 236, DATATBLS_SkillDescCalcLinker },
		{ "descline6", TXTFIELD_BYTE, 0, 71, NULL },
		{ "desctexta6", TXTFIELD_KEYTOWORD, 0, 94, DATATBLS_GetStringIdFromReferenceString },
		{ "desctextb6", TXTFIELD_KEYTOWORD, 0, 128, DATATBLS_GetStringIdFromReferenceString },
		{ "desccalca6", TXTFIELD_CALCTODWORD, 0, 172, DATATBLS_SkillDescCalcLinker },
		{ "desccalcb6", TXTFIELD_CALCTODWORD, 0, 240, DATATBLS_SkillDescCalcLinker },
		{ "dsc2line1", TXTFIELD_BYTE, 0, 72, NULL },
		{ "dsc2texta1", TXTFIELD_KEYTOWORD, 0, 96, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc2textb1", TXTFIELD_KEYTOWORD, 0, 130, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc2calca1", TXTFIELD_CALCTODWORD, 0, 176, DATATBLS_SkillDescCalcLinker },
		{ "dsc2calcb1", TXTFIELD_CALCTODWORD, 0, 244, DATATBLS_SkillDescCalcLinker },
		{ "dsc2line2", TXTFIELD_BYTE, 0, 73, NULL },
		{ "dsc2texta2", TXTFIELD_KEYTOWORD, 0, 98, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc2textb2", TXTFIELD_KEYTOWORD, 0, 132, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc2calca2", TXTFIELD_CALCTODWORD, 0, 180, DATATBLS_SkillDescCalcLinker },
		{ "dsc2calcb2", TXTFIELD_CALCTODWORD, 0, 248, DATATBLS_SkillDescCalcLinker },
		{ "dsc2line3", TXTFIELD_BYTE, 0, 74, NULL },
		{ "dsc2texta3", TXTFIELD_KEYTOWORD, 0, 100, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc2textb3", TXTFIELD_KEYTOWORD, 0, 134, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc2calca3", TXTFIELD_CALCTODWORD, 0, 184, DATATBLS_SkillDescCalcLinker },
		{ "dsc2calcb3", TXTFIELD_CALCTODWORD, 0, 252, DATATBLS_SkillDescCalcLinker },
		{ "dsc2line4", TXTFIELD_BYTE, 0, 75, NULL },
		{ "dsc2texta4", TXTFIELD_KEYTOWORD, 0, 102, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc2textb4", TXTFIELD_KEYTOWORD, 0, 136, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc2calca4", TXTFIELD_CALCTODWORD, 0, 188, DATATBLS_SkillDescCalcLinker },
		{ "dsc2calcb4", TXTFIELD_CALCTODWORD, 0, 256, DATATBLS_SkillDescCalcLinker },
		{ "dsc3line1", TXTFIELD_BYTE, 0, 76, NULL },
		{ "dsc3texta1", TXTFIELD_KEYTOWORD, 0, 104, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3textb1", TXTFIELD_KEYTOWORD, 0, 138, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3calca1", TXTFIELD_CALCTODWORD, 0, 192, DATATBLS_SkillDescCalcLinker },
		{ "dsc3calcb1", TXTFIELD_CALCTODWORD, 0, 260, DATATBLS_SkillDescCalcLinker },
		{ "dsc3line2", TXTFIELD_BYTE, 0, 77, NULL },
		{ "dsc3texta2", TXTFIELD_KEYTOWORD, 0, 106, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3textb2", TXTFIELD_KEYTOWORD, 0, 140, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3calca2", TXTFIELD_CALCTODWORD, 0, 196, DATATBLS_SkillDescCalcLinker },
		{ "dsc3calcb2", TXTFIELD_CALCTODWORD, 0, 264, DATATBLS_SkillDescCalcLinker },
		{ "dsc3line3", TXTFIELD_BYTE, 0, 78, NULL },
		{ "dsc3texta3", TXTFIELD_KEYTOWORD, 0, 108, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3textb3", TXTFIELD_KEYTOWORD, 0, 142, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3calca3", TXTFIELD_CALCTODWORD, 0, 200, DATATBLS_SkillDescCalcLinker },
		{ "dsc3calcb3", TXTFIELD_CALCTODWORD, 0, 268, DATATBLS_SkillDescCalcLinker },
		{ "dsc3line4", TXTFIELD_BYTE, 0, 79, NULL },
		{ "dsc3texta4", TXTFIELD_KEYTOWORD, 0, 110, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3textb4", TXTFIELD_KEYTOWORD, 0, 144, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3calca4", TXTFIELD_CALCTODWORD, 0, 204, DATATBLS_SkillDescCalcLinker },
		{ "dsc3calcb4", TXTFIELD_CALCTODWORD, 0, 272, DATATBLS_SkillDescCalcLinker },
		{ "dsc3line5", TXTFIELD_BYTE, 0, 80, NULL },
		{ "dsc3texta5", TXTFIELD_KEYTOWORD, 0, 112, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3textb5", TXTFIELD_KEYTOWORD, 0, 146, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3calca5", TXTFIELD_CALCTODWORD, 0, 208, DATATBLS_SkillDescCalcLinker },
		{ "dsc3calcb5", TXTFIELD_CALCTODWORD, 0, 276, DATATBLS_SkillDescCalcLinker },
		{ "dsc3line6", TXTFIELD_BYTE, 0, 81, NULL },
		{ "dsc3texta6", TXTFIELD_KEYTOWORD, 0, 114, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3textb6", TXTFIELD_KEYTOWORD, 0, 148, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3calca6", TXTFIELD_CALCTODWORD, 0, 212, DATATBLS_SkillDescCalcLinker },
		{ "dsc3calcb6", TXTFIELD_CALCTODWORD, 0, 280, DATATBLS_SkillDescCalcLinker },
		{ "dsc3line7", TXTFIELD_BYTE, 0, 82, NULL },
		{ "dsc3texta7", TXTFIELD_KEYTOWORD, 0, 116, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3textb7", TXTFIELD_KEYTOWORD, 0, 150, DATATBLS_GetStringIdFromReferenceString },
		{ "dsc3calca7", TXTFIELD_CALCTODWORD, 0, 216, DATATBLS_SkillDescCalcLinker },
		{ "dsc3calcb7", TXTFIELD_CALCTODWORD, 0, 284, DATATBLS_SkillDescCalcLinker },
		{ "end", 0, 0, 0, NULL },
	};

	if (sgptDataTables->bCompileTxt)
	{
		pRangeLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);

		FOG_10215(pRangeLinker, 'enon');
		FOG_10215(pRangeLinker, ' h2h');
		FOG_10215(pRangeLinker, ' gnr');
		FOG_10215(pRangeLinker, 'htob');
		FOG_10215(pRangeLinker, ' col');

		pMonStatsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		pTmpMonStatsTxt = DATATBLS_CompileTxt(pMemPool, "monstats", pTmpMonStatsTbl, &nSize, 2);

		pSkillDescLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		pTmpSkillDescTxt = DATATBLS_CompileTxt(pMemPool, "skilldesc", pTmpSkillDescTbl, &nSize, 2);
	}

	sgptDataTables->pSkillsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pSkillsTxt = (D2SkillsTxt*)DATATBLS_CompileTxt(pMemPool, "skills", pSkillTbl, &sgptDataTables->nSkillsTxtRecordCount, sizeof(D2SkillsTxt));
	if (sgptDataTables->nSkillsTxtRecordCount >= 32767)
	{
		FOG_10025("Skills table exceeded maximum number of entries.", __FILE__, __LINE__);
	}

	sgptDataTables->nPassiveSkills = 0;

	sgptDataTables->pSkillDescLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pSkillDescTxt = (D2SkillDescTxt*)DATATBLS_CompileTxt(pMemPool, "skilldesc", pSkillDescTbl, &sgptDataTables->nSkillDescTxtRecordCount, sizeof(D2SkillDescTxt));
	if (sgptDataTables->nSkillDescTxtRecordCount >= 32767)
	{
		FOG_10025("SkillDesc table exceeded maximum number of entries.", __FILE__, __LINE__);
	}

	if (sgptDataTables->bCompileTxt)
	{
		FOG_FreeLinker(pRangeLinker);
		FOG_FreeLinker(pMonStatsLinker);

		if (pTmpMonStatsTxt)
		{
			if (dword_6FDD6A24)
			{
				FOG_FreeServerMemory(NULL, (char*)pTmpMonStatsTxt - 4, __FILE__, __LINE__, 0);

			}
			else
			{
				FOG_FreeServerMemory(NULL, pTmpMonStatsTxt, __FILE__, __LINE__, 0);
			}
		}

		FOG_FreeLinker(pSkillDescLinker);

		if (pTmpSkillDescTxt)
		{
			if (dword_6FDD6A24)
			{
				FOG_FreeServerMemory(NULL, (char*)pTmpSkillDescTxt - 4, __FILE__, __LINE__, 0);
			}
			else
			{
				FOG_FreeServerMemory(NULL, pTmpSkillDescTxt, __FILE__, __LINE__, 0);
			}
		}

		if (sgptDataTables->bCompileTxt && sgptDataTables->pSkillsCode)
		{
			wsprintfA(szFileName, "%s\\%s.bin", "DATA\\GLOBAL\\EXCEL", "skillscode");
			fopen_s(&pSkillsCodeBin, szFileName, "wb");
			if (pSkillsCodeBin)
			{
				DATATBLS_LockAndWriteToFile(sgptDataTables->pSkillsCode, sgptDataTables->nSkillsCodeSize, 1u, pSkillsCodeBin);
				fclose(pSkillsCodeBin);
			}
			FOG_FreeServerMemory(NULL, sgptDataTables->pSkillsCode, __FILE__, __LINE__, 0);
		}
	}

	wsprintfA(szFileName, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", "skillscode", ".bin");
	sgptDataTables->pSkillsCode = (char*)DATATBLS_GetBinaryData(pMemPool, szFileName, &nSize, __FILE__, __LINE__);
	sgptDataTables->nSkillsCodeSizeEx = nSize;
	sgptDataTables->nSkillsCodeSize = nSize;

	if (sgptDataTables->bCompileTxt && sgptDataTables->pSkillDescCode)
	{
		wsprintfA(szFileName, "%s\\%s.bin", "DATA\\GLOBAL\\EXCEL", "skilldesccode");
		fopen_s(&pSkillDescCodeBin, szFileName, "wb");
		if (pSkillDescCodeBin)
		{
			DATATBLS_LockAndWriteToFile(sgptDataTables->pSkillDescCode, sgptDataTables->nSkillDescCodeSize, 1, pSkillDescCodeBin);
			fclose(pSkillDescCodeBin);
		}
		FOG_FreeServerMemory(NULL, sgptDataTables->pSkillDescCode, __FILE__, __LINE__, 0);
	}

	wsprintfA(szFileName, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", "skilldesccode", ".bin");
	sgptDataTables->pSkillDescCode = (char*)DATATBLS_GetBinaryData(pMemPool, szFileName, &nSize, __FILE__, __LINE__);
	sgptDataTables->nSkillDescCodeSizeEx = nSize;
	sgptDataTables->nSkillDescCodeSize = nSize;

	sgptDataTables->nClassSkillCount = (int*)FOG_AllocServerMemory(NULL, 7 * sizeof(int), __FILE__, __LINE__, 0);
	memset(sgptDataTables->nClassSkillCount, 0x00, 7 * sizeof(int));

	for (int i = 0; i < sgptDataTables->nSkillsTxtRecordCount; ++i)
	{
		nClass = sgptDataTables->pSkillsTxt[i].nCharClass;
		if (nClass >= 0 && nClass < 7)
		{
			++sgptDataTables->nClassSkillCount[nClass];
		}

		if (sgptDataTables->pSkillsTxt[i].nPassiveState >= 0)
		{
			++sgptDataTables->nPassiveSkills;
		}
	}

	nHighestClassSkillCount = 0;
	sgptDataTables->nHighestClassSkillCount = 0;

	for (int i = 0; i < 7; ++i)
	{
		if (sgptDataTables->nClassSkillCount[i] > nHighestClassSkillCount)
		{
			nHighestClassSkillCount = sgptDataTables->nClassSkillCount[i];
			sgptDataTables->nHighestClassSkillCount = sgptDataTables->nClassSkillCount[i];
		}
	}

	sgptDataTables->nClassSkillList = (short*)FOG_AllocServerMemory(NULL, 7 * sizeof(short) * nHighestClassSkillCount, __FILE__, __LINE__, 0);
	memset(sgptDataTables->nClassSkillList, 0x00, 7 * sizeof(short) * nHighestClassSkillCount);
	memset(sgptDataTables->nClassSkillCount, 0x00, 7 * sizeof(int));

	sgptDataTables->pPassiveSkills = (uint16_t*)FOG_AllocServerMemory(NULL, sizeof(uint16_t) * sgptDataTables->nPassiveSkills, __FILE__, __LINE__, 0);
	memset(sgptDataTables->pPassiveSkills, 0x00, sizeof(uint16_t) * sgptDataTables->nPassiveSkills);

	sgptDataTables->nPassiveSkills = 0;
	for (int i = 0; i < sgptDataTables->nSkillsTxtRecordCount; ++i)
	{
		nClass = sgptDataTables->pSkillsTxt[i].nCharClass;
		if (nClass >= 0 && nClass < 7)
		{
			sgptDataTables->nClassSkillList[sgptDataTables->nClassSkillCount[nClass] + sgptDataTables->nHighestClassSkillCount * nClass] = i;
			++sgptDataTables->nClassSkillCount[nClass];
		}

		if (sgptDataTables->pSkillsTxt[i].nPassiveState >= 0)
		{
			sgptDataTables->pPassiveSkills[sgptDataTables->nPassiveSkills] = i;
			++sgptDataTables->nPassiveSkills;
		}

		nPetType = sgptDataTables->pSkillsTxt[i].nPetType;
		if (nPetType >= 0 && nPetType < sgptDataTables->nPetTypeTxtRecordCount)
		{
			pPetTypeTxtRecord = &sgptDataTables->pPetTypeTxt[nPetType];
			if (pPetTypeTxtRecord->nSkillCount < ARRAY_SIZE(pPetTypeTxtRecord->wSkillIds))
			{
				pPetTypeTxtRecord->wSkillIds[pPetTypeTxtRecord->nSkillCount] = i;
				++pPetTypeTxtRecord->nSkillCount;
			}
		}
	}
}

//D2Common.0x6FD4E350
void __fastcall DATATBLS_UnloadSkills_SkillDescTxt()
{
	if (sgptDataTables->nClassSkillCount)
	{
		FOG_FreeServerMemory(NULL, sgptDataTables->nClassSkillCount, __FILE__, __LINE__, 0);
		sgptDataTables->nClassSkillCount = 0;
	}

	if (sgptDataTables->nClassSkillList)
	{
		FOG_FreeServerMemory(NULL, sgptDataTables->nClassSkillList, __FILE__, __LINE__, 0);
		sgptDataTables->nClassSkillList = 0;
	}
	sgptDataTables->nHighestClassSkillCount = 0;

	if (sgptDataTables->pPassiveSkills)
	{
		FOG_FreeServerMemory(NULL, sgptDataTables->pPassiveSkills, __FILE__, __LINE__, 0);
		sgptDataTables->pPassiveSkills = NULL;
	}
	sgptDataTables->nPassiveSkills = 0;

	if (sgptDataTables->pSkillsCode)
	{
		FOG_FreeServerMemory(NULL, sgptDataTables->pSkillsCode, __FILE__, __LINE__, 0);
		sgptDataTables->pSkillsCode = NULL;
	}
	sgptDataTables->nSkillsCodeSize = 0;
	sgptDataTables->nSkillsCodeSizeEx = 0;

	if (sgptDataTables->pSkillDescCode)
	{
		FOG_FreeServerMemory(NULL, sgptDataTables->pSkillDescCode, __FILE__, __LINE__, 0);
		sgptDataTables->pSkillDescCode = NULL;
	}
	sgptDataTables->nSkillDescCodeSize = 0;
	sgptDataTables->nSkillDescCodeSizeEx = 0;

	DATATBLS_UnloadBin(sgptDataTables->pSkillsTxt);
	sgptDataTables->pSkillsTxt = NULL;
	FOG_FreeLinker(sgptDataTables->pSkillsLinker);
	sgptDataTables->pSkillsLinker = NULL;
	sgptDataTables->nSkillsTxtRecordCount = 0;

	DATATBLS_UnloadBin(sgptDataTables->pSkillDescTxt);
	sgptDataTables->pSkillDescTxt = NULL;
	FOG_FreeLinker(sgptDataTables->pSkillDescLinker);
	sgptDataTables->pSkillDescLinker = NULL;
	sgptDataTables->nSkillDescTxtRecordCount = 0;
}

//Inlined at various places
D2SkillsTxt* __fastcall DATATBLS_GetSkillsTxtRecord(int nSkillId)
{
	if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
	{
		return &sgptDataTables->pSkillsTxt[nSkillId];
	}

	return NULL;
}

//Inlined at various places
D2SkillDescTxt* __fastcall DATATBLS_GetSkillDescTxtRecord(int nSkillDesc)
{
	if (nSkillDesc >= 0 && nSkillDesc < sgptDataTables->nSkillDescTxtRecordCount)
	{
		return &sgptDataTables->pSkillDescTxt[nSkillDesc];
	}

	return NULL;
}
