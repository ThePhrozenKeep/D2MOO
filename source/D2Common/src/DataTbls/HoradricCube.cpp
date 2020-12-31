#include "D2DataTbls.h"


// Inlined in both Parsers
static BOOL DATATBLS_AreStringsEqual(const char* szString1, const char* szString2)
{
	for (size_t i = strlen(szString2) + 1; i; --i)
	{
		if (*szString1++ != *szString2++)
		{
			return FALSE;
		}
	}

	return TRUE;
}


//D2Common.0x6FD523E0
void __fastcall DATATBLS_CubeMainInputLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	if (pRecord && pSrc && *pSrc)
	{
		DATATBLS_CubeMainInputParser(&((D2CubeMainTxt*)pRecord)->pInputItem[nOffset], pSrc, nTxtRow, nOffset);
	}
}

//D2Common.0x6FD52410
BOOL __fastcall DATATBLS_CubeMainInputParser(D2CubeInputItem* pCubeInput, char* szInput, int nTxtRow, int nItemId)
{
	char* szInputModifier = NULL;
	char* szNext = NULL;
	char* szTemp = NULL;
	int nBaseItemId = 0;
	int nLinkId = 0;

	if (*szInput == '"')
	{
		++szInput;
	}

	szTemp = szInput;
	while (*szTemp)
	{
		if (*szTemp == '"')
		{
			*szTemp = 0;
		}
		++szTemp;
	}

	if (*szInput)
	{
		szNext = &szInput[strcspn(szInput, ",")];
		if (*szNext)
		{
			*szNext = 0;
			++szNext;
		}
		else
		{
			szNext = NULL;
		}

		if (strlen(szInput) <= 4 && !_strcmpi(szInput, "any"))
		{
			pCubeInput->wInputFlags |= CUBEFLAG_IN_USEANY;
			pCubeInput->wItem = -1;
		}
		else
		{
			nLinkId = -1;

			if (strlen(szInput) <= 4)
			{
				nLinkId = FOG_GetLinkIndex(sgptDataTables->pItemTypesLinker, DATATBLS_StringToCode(szInput), 0);
			}

			if (nLinkId >= 0)
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_ITEMCODE;
				pCubeInput->wItem = nLinkId;
			}
			else if (strlen(szInput) <= 4 && DATATBLS_GetItemRecordFromItemCode(DATATBLS_StringToCode(szInput), &nBaseItemId))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_USEANY;
				pCubeInput->wItem = nBaseItemId;
			}
			else
			{
				nLinkId = -1;

				if (sgptDataTables->pUniqueItemsLinker)
				{
					nLinkId = FOG_GetRowFromTxt(sgptDataTables->pUniqueItemsLinker, szInput, 0);
				}

				if (nLinkId >= 0)
				{
					DATATBLS_GetItemRecordFromItemCode(sgptDataTables->pUniqueItemsTxt[nLinkId].dwBaseItemCode, &nBaseItemId);
					pCubeInput->wInputFlags |= CUBEFLAG_IN_SPECIAL | CUBEFLAG_IN_USEANY;
					pCubeInput->nQuality = ITEMQUAL_UNIQUE;
					pCubeInput->wItemID = nLinkId + 1;
					pCubeInput->wItem = nBaseItemId;
				}
				else
				{
					nLinkId = -1;

					if (sgptDataTables->pSetItemsLinker)
					{
						nLinkId = FOG_GetRowFromTxt(sgptDataTables->pSetItemsLinker, szInput, 0);
					}

					if (nLinkId >= 0)
					{
						DATATBLS_GetItemRecordFromItemCode(sgptDataTables->pSetItemsTxt[nLinkId].szItemCode, &nBaseItemId);
						pCubeInput->wInputFlags |= CUBEFLAG_IN_SPECIAL | CUBEFLAG_IN_USEANY;
						pCubeInput->nQuality = ITEMQUAL_SET;
						pCubeInput->wItemID = nLinkId + 1;
						pCubeInput->wItem = nBaseItemId;
					}
					else
					{
						return FALSE;
					}
				}
			}
		}

		while (szNext)
		{
			szInputModifier = szNext;
			if (!szInputModifier)
			{
				return TRUE;
			}

			szNext = &szNext[strcspn(szNext, "=,")];
			if (*szNext)
			{
				*szNext = 0;
				++szNext;
			}
			else
			{
				szNext = NULL;
			}

			if (DATATBLS_AreStringsEqual(szInputModifier, "qty"))
			{
				szTemp = szNext;
				szNext = &szNext[strcspn(szNext, ",")];
				if (*szNext)
				{
					*szNext = 0;
					++szNext;
					pCubeInput->nQuantity = atoi(szTemp);
				}
				else
				{
					szNext = NULL;
					pCubeInput->nQuantity = atoi(szTemp);
				}
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "low"))
			{
				pCubeInput->nQuality = ITEMQUAL_INFERIOR;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "nor"))
			{
				pCubeInput->nQuality = ITEMQUAL_NORMAL;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "hiq"))
			{
				pCubeInput->nQuality = ITEMQUAL_SUPERIOR;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "mag"))
			{
				pCubeInput->nQuality = ITEMQUAL_MAGIC;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "set"))
			{
				pCubeInput->nQuality = ITEMQUAL_SET;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "rar"))
			{
				pCubeInput->nQuality = ITEMQUAL_RARE;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "uni"))
			{
				pCubeInput->nQuality = ITEMQUAL_UNIQUE;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "crf"))
			{
				pCubeInput->nQuality = ITEMQUAL_CRAFT;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "tmp"))
			{
				pCubeInput->nQuality = ITEMQUAL_TEMPERED;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "nos"))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_NOSOCKET;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "sock"))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_SOCKETED;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "noe"))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_NOETHEREAL;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "eth"))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_ETHEREAL;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "upg"))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_UPGRADED;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "bas"))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_NORMAL;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "exc"))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_EXCEPTIONAL;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "eli"))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_ELITE;
			}
			else if (DATATBLS_AreStringsEqual(szInputModifier, "nru"))
			{
				pCubeInput->wInputFlags |= CUBEFLAG_IN_NORUNES;
			}
			else
			{
				return TRUE;
			}
		}
	}

	return TRUE;
}

//D2Common.0x6FD528D0
void __fastcall DATATBLS_CubeMainOutputLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	if (pRecord && pSrc && *pSrc)
	{
		DATATBLS_CubeMainOutputParser(&((D2CubeMainTxt*)pRecord)->pOutputItem[nOffset], pSrc, nTxtRow, nOffset);
	}
}

//D2Common.0x6FD52910
BOOL __fastcall DATATBLS_CubeMainOutputParser(D2CubeOutputItem* pCubeOutputParam, char* szOutput, int nTxtRow, int nItemId)
{
	char* szOutputModifier = NULL;
	char* szNext = NULL;
	char* szTemp = NULL;
	uint16_t* pPrefix = NULL;
	uint16_t* pSuffix = NULL;
	int nBaseItemId = 0;
	int nLinkId = 0;

	if (*szOutput == '"')
	{
		++szOutput;
	}

	szTemp = szOutput;
	while (*szTemp)
	{
		if (*szTemp == '"')
		{
			*szTemp = 0;
		}
		++szTemp;
	}

	szNext = &szOutput[strcspn(szOutput, ",")];
	if (*szNext)
	{
		*szNext = 0;
		++szNext;
	}
	else
	{
		szNext = NULL;
	}

	if (!_strcmpi(szOutput, "Cow Portal"))
	{
		pCubeOutputParam->nType = CUBEOP_COWPORTAL;
	}
	else if (!_strcmpi(szOutput, "Pandemonium Portal"))
	{
		pCubeOutputParam->nType = CUBEOP_UBERDUNGEON;
	}
	else if (!_strcmpi(szOutput, "Pandemonium Finale Portal"))
	{
		pCubeOutputParam->nType = CUBEOP_UBERTRISTRAM;
	}
	else if (DATATBLS_AreStringsEqual(szOutput, "usetype"))
	{
		pCubeOutputParam->nType = CUBEOP_USETYPE;
	}
	else if (DATATBLS_AreStringsEqual(szOutput, "useitem"))
	{
		pCubeOutputParam->nType = CUBEOP_USEITEM;
	}
	else if (strlen(szOutput) <= 4 && DATATBLS_GetItemRecordFromItemCode(DATATBLS_StringToCode(szOutput), &nBaseItemId))
	{
		pCubeOutputParam->nType = CUBEOP_ITEMCODE;
		pCubeOutputParam->wBaseItemId = nBaseItemId;
	}
	else
	{
		nLinkId = -1;

		if (strlen(szOutput) <= 4)
		{
			nLinkId = FOG_GetLinkIndex(sgptDataTables->pItemTypesLinker, DATATBLS_StringToCode(szOutput), 0);
		}

		if (nLinkId >= 0)
		{
			pCubeOutputParam->nType = CUBEOP_ITEMTYPE;
			pCubeOutputParam->wBaseItemId = nLinkId;
		}
		else
		{
			nLinkId = -1;

			if (sgptDataTables->pUniqueItemsLinker)
			{
				nLinkId = FOG_GetRowFromTxt(sgptDataTables->pUniqueItemsLinker, szOutput, 0);
			}

			if (nLinkId >= 0)
			{
				DATATBLS_GetItemRecordFromItemCode(sgptDataTables->pUniqueItemsTxt[nLinkId].dwBaseItemCode, &nBaseItemId);
				pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_SPECIAL;
				pCubeOutputParam->nType = CUBEOP_ITEMCODE;
				pCubeOutputParam->nQuality = ITEMQUAL_UNIQUE;
				pCubeOutputParam->wItemID = nLinkId + 1;
				pCubeOutputParam->wBaseItemId = nBaseItemId;
				pCubeOutputParam->nILvl = (uint8_t)sgptDataTables->pUniqueItemsTxt[nLinkId].wLvl;
			}
			else
			{
				nLinkId = -1;

				if (sgptDataTables->pSetItemsLinker)
				{
					nLinkId = FOG_GetRowFromTxt(sgptDataTables->pSetItemsLinker, szOutput, 0);
				}

				if (nLinkId >= 0)
				{
					DATATBLS_GetItemRecordFromItemCode(sgptDataTables->pSetItemsTxt[nLinkId].szItemCode, &nBaseItemId);
					pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_SPECIAL;
					pCubeOutputParam->nType = CUBEOP_ITEMCODE;
					pCubeOutputParam->nQuality = ITEMQUAL_SET;
					pCubeOutputParam->wItemID = nLinkId + 1;
					pCubeOutputParam->wBaseItemId = nBaseItemId;
					pCubeOutputParam->nILvl = (uint8_t)sgptDataTables->pSetItemsTxt[nLinkId].wLvl;
				}
				else
				{
					FOG_WriteToLogFile("Couldn't parse horardric cube output!  (Line:%d  Item:%d)\n", nTxtRow, nItemId);
					return FALSE;
				}
			}
		}
	}

	pPrefix = pCubeOutputParam->wPrefixId;
	pSuffix = pCubeOutputParam->wSuffixId;

	while (szNext)
	{
		szOutputModifier = szNext;

		szNext = &szNext[strcspn(szNext, "=,")];
		if (*szNext)
		{
			*szNext = 0;
			++szNext;
		}
		else
		{
			szNext = NULL;
		}

		if (!szOutputModifier)
		{
			FOG_WriteToLogFile("Couldn't parse horardric cube output modifier!  (Line:%d  Item:%d)\n", nTxtRow, nItemId);
			return TRUE;
		}

		if (DATATBLS_AreStringsEqual(szOutputModifier, "qty"))
		{
			szTemp = szNext;

			szNext = &szNext[strcspn(szNext, ",")];
			if (*szNext)
			{
				*szNext = 0;
				++szNext;
			}
			else
			{
				szNext = NULL;
			}
			pCubeOutputParam->nQuantity = atoi(szTemp);
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "pre"))
		{
			szTemp = szNext;

			szNext = &szNext[strcspn(szNext, ",")];
			if (*szNext)
			{
				*szNext = 0;
				++szNext;
			}
			else
			{
				szNext = NULL;
			}
			*pPrefix = atoi(szTemp);
			++pPrefix;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "suf"))
		{
			szTemp = szNext;

			szNext = &szNext[strcspn(szNext, ",")];
			if (*szNext)
			{
				*szNext = 0;
				++szNext;
			}
			else
			{
				szNext = NULL;
			}
			*pSuffix = atoi(szTemp);
			++pSuffix;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "low"))
		{
			pCubeOutputParam->nQuality = ITEMQUAL_INFERIOR;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "nor"))
		{
			pCubeOutputParam->nQuality = ITEMQUAL_NORMAL;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "hiq"))
		{
			pCubeOutputParam->nQuality = ITEMQUAL_SUPERIOR;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "mag"))
		{
			pCubeOutputParam->nQuality = ITEMQUAL_MAGIC;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "set"))
		{
			pCubeOutputParam->nQuality = ITEMQUAL_SET;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "rar"))
		{
			pCubeOutputParam->nQuality = ITEMQUAL_RARE;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "uni"))
		{
			pCubeOutputParam->nQuality = ITEMQUAL_UNIQUE;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "crf"))
		{
			pCubeOutputParam->nQuality = ITEMQUAL_CRAFT;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "tmp"))
		{
			pCubeOutputParam->nQuality = ITEMQUAL_TEMPERED;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "eth"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_ETHEREAL;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "sock"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_SOCKET;

			szTemp = szNext;

			szNext = &szNext[strcspn(szNext, ",")];
			if (*szNext)
			{
				*szNext = 0;
				++szNext;
			}
			else
			{
				szNext = NULL;
			}
			pCubeOutputParam->nQuantity = atoi(szTemp);
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "mod"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_COPYMODS;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "uns"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_UNSOCKET;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "rem"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_REMOVE;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "reg"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_NORMAL;
			pCubeOutputParam->nType = CUBEOP_USETYPE;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "exc"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_EXCEPTIONAL;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "eli"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_ELITE;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "rep"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_REPAIR;
		}
		else if (DATATBLS_AreStringsEqual(szOutputModifier, "rch"))
		{
			pCubeOutputParam->wItemFlags |= CUBEFLAG_OUT_RECHARGE;
		}
		else
		{
			FOG_WriteToLogFile("Couldn't parse horardric cube output modifier '%s'!  (Line:%d  Item:%d)\n", szOutputModifier, nTxtRow, nItemId);
			return TRUE;
		}
	}

	return TRUE;
}

//D2Common.0x6FD52FC0
void __fastcall DATATBLS_CubeMainParamLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	int nValue = 0;

	if (pRecord && pSrc && *pSrc)
	{
		if (*pSrc == '-' || *pSrc >= '0' && *pSrc <= '9')
		{
			*(int*)pRecord = atoi(pSrc);
		}
		else
		{
			if (sgptDataTables->pItemStatCostLinker)
			{
				nValue = FOG_GetRowFromTxt(sgptDataTables->pItemStatCostLinker, pSrc, 0);
				if (nValue >= 0)
				{
					*(int*)pRecord = nValue;
					return;
				}
			}

			*(int*)pRecord = 0;
			FOG_WriteToLogFile("Failed to parse '%s' line %d", pSrc, nTxtRow);
		}
	}
}

//D2Common.0x6FD53030
void __fastcall DATATBLS_LoadCubeMainTxt(void* pMemPool)
{
	void* pFileHandle = NULL;
	char szPath[100] = {};

	D2BinFieldStrc pTbl[] =
	{
		{ "enabled", TXTFIELD_BYTE, 0, 0, NULL },
		{ "ladder", TXTFIELD_BYTE, 0, 1, NULL },
		{ "min diff", TXTFIELD_BYTE, 0, 2, NULL },
		{ "op", TXTFIELD_BYTE, 0, 4, NULL },
		{ "param", TXTFIELD_DWORD, 0, 8, DATATBLS_CubeMainParamLinker },
		{ "value", TXTFIELD_DWORD, 0, 12, NULL },
		{ "class", TXTFIELD_CODETOBYTE, 0, 3, &sgptDataTables->pPlayerClassLinker },
		{ "numinputs", TXTFIELD_BYTE, 0, 16, NULL },
		{ "version", TXTFIELD_WORD, 0, 18, NULL },
		{ "input 1", TXTFIELD_CUSTOMLINK, 0, 0, DATATBLS_CubeMainInputLinker },
		{ "input 2", TXTFIELD_CUSTOMLINK, 0, 1, DATATBLS_CubeMainInputLinker },
		{ "input 3", TXTFIELD_CUSTOMLINK, 0, 2, DATATBLS_CubeMainInputLinker },
		{ "input 4", TXTFIELD_CUSTOMLINK, 0, 3, DATATBLS_CubeMainInputLinker },
		{ "input 5", TXTFIELD_CUSTOMLINK, 0, 4, DATATBLS_CubeMainInputLinker },
		{ "input 6", TXTFIELD_CUSTOMLINK, 0, 5, DATATBLS_CubeMainInputLinker },
		{ "input 7", TXTFIELD_CUSTOMLINK, 0, 6, DATATBLS_CubeMainInputLinker },
		{ "output", TXTFIELD_CUSTOMLINK, 0, 0, DATATBLS_CubeMainOutputLinker },
		{ "lvl", TXTFIELD_BYTE, 0, 85, NULL },
		{ "plvl", TXTFIELD_BYTE, 0, 86, NULL },
		{ "ilvl", TXTFIELD_BYTE, 0, 87, NULL },
		{ "mod 1", TXTFIELD_NAMETODWORD, 0, 100, &sgptDataTables->pPropertiesLinker },
		{ "mod 1 chance", TXTFIELD_BYTE, 0, 110, NULL },
		{ "mod 1 param", TXTFIELD_WORD, 0, 104, NULL },
		{ "mod 1 min", TXTFIELD_WORD, 0, 106, NULL },
		{ "mod 1 max", TXTFIELD_WORD, 0, 108, NULL },
		{ "mod 2", TXTFIELD_NAMETODWORD, 0, 112, &sgptDataTables->pPropertiesLinker },
		{ "mod 2 chance", TXTFIELD_BYTE, 0, 122, NULL },
		{ "mod 2 param", TXTFIELD_WORD, 0, 116, NULL },
		{ "mod 2 min", TXTFIELD_WORD, 0, 118, NULL },
		{ "mod 2 max", TXTFIELD_WORD, 0, 120, NULL },
		{ "mod 3", TXTFIELD_NAMETODWORD, 0, 124, &sgptDataTables->pPropertiesLinker },
		{ "mod 3 chance", TXTFIELD_BYTE, 0, 134, NULL },
		{ "mod 3 param", TXTFIELD_WORD, 0, 128, NULL },
		{ "mod 3 min", TXTFIELD_WORD, 0, 130, NULL },
		{ "mod 3 max", TXTFIELD_WORD, 0, 132, NULL },
		{ "mod 4", TXTFIELD_NAMETODWORD, 0, 136, &sgptDataTables->pPropertiesLinker },
		{ "mod 4 chance", TXTFIELD_BYTE, 0, 146, NULL },
		{ "mod 4 param", TXTFIELD_WORD, 0, 140, NULL },
		{ "mod 4 min", TXTFIELD_WORD, 0, 142, NULL },
		{ "mod 4 max", TXTFIELD_WORD, 0, 144, NULL },
		{ "mod 5", TXTFIELD_NAMETODWORD, 0, 148, &sgptDataTables->pPropertiesLinker },
		{ "mod 5 chance", TXTFIELD_BYTE, 0, 158, NULL },
		{ "mod 5 param", TXTFIELD_WORD, 0, 152, NULL },
		{ "mod 5 min", TXTFIELD_WORD, 0, 154, NULL },
		{ "mod 5 max", TXTFIELD_WORD, 0, 156, NULL },
		{ "output b", TXTFIELD_CUSTOMLINK, 0, 1, DATATBLS_CubeMainOutputLinker },
		{ "b lvl", TXTFIELD_BYTE, 0, 169, NULL },
		{ "b plvl", TXTFIELD_BYTE, 0, 170, NULL },
		{ "b ilvl", TXTFIELD_BYTE, 0, 171, NULL },
		{ "b mod 1", TXTFIELD_NAMETODWORD, 0, 184, &sgptDataTables->pPropertiesLinker },
		{ "b mod 1 chance", TXTFIELD_BYTE, 0, 194, NULL },
		{ "b mod 1 param", TXTFIELD_WORD, 0, 188, NULL },
		{ "b mod 1 min", TXTFIELD_WORD, 0, 190, NULL },
		{ "b mod 1 max", TXTFIELD_WORD, 0, 192, NULL },
		{ "b mod 2", TXTFIELD_NAMETODWORD, 0, 196, &sgptDataTables->pPropertiesLinker },
		{ "b mod 2 chance", TXTFIELD_BYTE, 0, 206, NULL },
		{ "b mod 2 param", TXTFIELD_WORD, 0, 200, NULL },
		{ "b mod 2 min", TXTFIELD_WORD, 0, 202, NULL },
		{ "b mod 2 max", TXTFIELD_WORD, 0, 204, NULL },
		{ "b mod 3", TXTFIELD_NAMETODWORD, 0, 208, &sgptDataTables->pPropertiesLinker },
		{ "b mod 3 chance", TXTFIELD_BYTE, 0, 218, NULL },
		{ "b mod 3 param", TXTFIELD_WORD, 0, 212, NULL },
		{ "b mod 3 min", TXTFIELD_WORD, 0, 214, NULL },
		{ "b mod 3 max", TXTFIELD_WORD, 0, 216, NULL },
		{ "b mod 4", TXTFIELD_NAMETODWORD, 0, 220, &sgptDataTables->pPropertiesLinker },
		{ "b mod 4 chance", TXTFIELD_BYTE, 0, 230, NULL },
		{ "b mod 4 param", TXTFIELD_WORD, 0, 224, NULL },
		{ "b mod 4 min", TXTFIELD_WORD, 0, 226, NULL },
		{ "b mod 4 max", TXTFIELD_WORD, 0, 228, NULL },
		{ "b mod 5", TXTFIELD_NAMETODWORD, 0, 232, &sgptDataTables->pPropertiesLinker },
		{ "b mod 5 chance", TXTFIELD_BYTE, 0, 242, NULL },
		{ "b mod 5 param", TXTFIELD_WORD, 0, 236, NULL },
		{ "b mod 5 min", TXTFIELD_WORD, 0, 238, NULL },
		{ "b mod 5 max", TXTFIELD_WORD, 0, 240, NULL },
		{ "output c", TXTFIELD_CUSTOMLINK, 0, TXTFIELD_DWORD, DATATBLS_CubeMainOutputLinker },
		{ "c lvl", TXTFIELD_BYTE, 0, 253, NULL },
		{ "c plvl", TXTFIELD_BYTE, 0, 254, NULL },
		{ "c ilvl", TXTFIELD_BYTE, 0, 255, NULL },
		{ "c mod 1", TXTFIELD_NAMETODWORD, 0, 268, &sgptDataTables->pPropertiesLinker },
		{ "c mod 1 chance", TXTFIELD_BYTE, 0, 278, NULL },
		{ "c mod 1 param", TXTFIELD_WORD, 0, 272, NULL },
		{ "c mod 1 min", TXTFIELD_WORD, 0, 274, NULL },
		{ "c mod 1 max", TXTFIELD_WORD, 0, 276, NULL },
		{ "c mod 2", TXTFIELD_NAMETODWORD, 0, 280, &sgptDataTables->pPropertiesLinker },
		{ "c mod 2 chance", TXTFIELD_BYTE, 0, 290, NULL },
		{ "c mod 2 param", TXTFIELD_WORD, 0, 284, NULL },
		{ "c mod 2 min", TXTFIELD_WORD, 0, 286, NULL },
		{ "c mod 2 max", TXTFIELD_WORD, 0, 288, NULL },
		{ "c mod 3", TXTFIELD_NAMETODWORD, 0, 292, &sgptDataTables->pPropertiesLinker },
		{ "c mod 3 chance", TXTFIELD_BYTE, 0, 302, NULL },
		{ "c mod 3 param", TXTFIELD_WORD, 0, 296, NULL },
		{ "c mod 3 min", TXTFIELD_WORD, 0, 298, NULL },
		{ "c mod 3 max", TXTFIELD_WORD, 0, 300, NULL },
		{ "c mod 4", TXTFIELD_NAMETODWORD, 0, 304, &sgptDataTables->pPropertiesLinker },
		{ "c mod 4 chance", TXTFIELD_BYTE, 0, 314, NULL },
		{ "c mod 4 param", TXTFIELD_WORD, 0, 308, NULL },
		{ "c mod 4 min", TXTFIELD_WORD, 0, 310, NULL },
		{ "c mod 4 max", TXTFIELD_WORD, 0, 312, NULL },
		{ "c mod 5", TXTFIELD_NAMETODWORD, 0, 316, &sgptDataTables->pPropertiesLinker },
		{ "c mod 5 chance", TXTFIELD_BYTE, 0, 326, NULL },
		{ "c mod 5 param", TXTFIELD_WORD, 0, 320, NULL },
		{ "c mod 5 min", TXTFIELD_WORD, 0, 322, NULL },
		{ "c mod 5 max", TXTFIELD_WORD, 0, 324, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	wsprintfA(szPath, "%s\\%s", "DATA\\GLOBAL\\EXCEL", "cubeserver.bin");
	if (DATATBLS_CheckIfFileExists(pMemPool, szPath, &pFileHandle, 1))
	{
		FOG_10025("Found cubeserver.bin in data path.  This file should only be on the server\n", __FILE__, __LINE__);
	}

	wsprintfA(szPath, "%s\\%s", "DATA\\GLOBAL\\EXCEL", "cubeserver.txt");
	if (DATATBLS_CheckIfFileExists(pMemPool, szPath, &pFileHandle, 1))
	{
		FOG_10025("Found cubeserver.txt in data path.  This file should only be on the server\n", __FILE__, __LINE__);
	}

	sgptDataTables->pCubeMainTxt = (D2CubeMainTxt*)DATATBLS_CompileTxt(pMemPool, "cubemain", pTbl, &sgptDataTables->nCubeMainTxtRecordCount, sizeof(D2CubeMainTxt));
}

//D2Common.0x6FD54250
void __fastcall DATATBLS_UnloadCubeMainTxt()
{
	DATATBLS_UnloadBin(sgptDataTables->pCubeMainTxt);
	sgptDataTables->pCubeMainTxt = NULL;
}

//D2Common.0x6FD54260 (#11232)
D2CubeMainTxt* __stdcall DATATBLS_GetCubemainTxtRecord(int nIndex)
{
	D2_ASSERT(sgptDataTables->pCubeMainTxt);
	D2_ASSERT(nIndex < sgptDataTables->nCubeMainTxtRecordCount);

	return &sgptDataTables->pCubeMainTxt[nIndex];
}

//D2Common.0x6FD542C0 (#11233)
int __fastcall DATATBLS_GetCubemainTxtRecordCount()
{
	return sgptDataTables->nCubeMainTxtRecordCount;
}
