#include "D2DataTbls.h"

#include "D2Items.h"


//D2Common.0x6FD542D0
void __fastcall DATATBLS_LoadInventoryTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "invLeft", TXTFIELD_DWORD, 0, 0, NULL },
		{ "invRight", TXTFIELD_DWORD, 0, 4, NULL },
		{ "invTop", TXTFIELD_DWORD, 0, 8, NULL },
		{ "invBottom", TXTFIELD_DWORD, 0, 12, NULL },
		{ "gridX", TXTFIELD_BYTE, 0, 16, NULL },
		{ "gridY", TXTFIELD_BYTE, 0, 17, NULL },
		{ "gridLeft", TXTFIELD_DWORD, 0, 20, NULL },
		{ "gridRight", TXTFIELD_DWORD, 0, 24, NULL },
		{ "gridTop", TXTFIELD_DWORD, 0, 28, NULL },
		{ "gridBottom", TXTFIELD_DWORD, 0, 32, NULL },
		{ "gridBoxWidth", TXTFIELD_BYTE, 0, 36, NULL },
		{ "gridBoxHeight", TXTFIELD_BYTE, 0, 37, NULL },
		{ "rArmLeft", TXTFIELD_DWORD, 0, 40, NULL },
		{ "rArmRight", TXTFIELD_DWORD, 0, 44, NULL },
		{ "rArmTop", TXTFIELD_DWORD, 0, 48, NULL },
		{ "rArmBottom", TXTFIELD_DWORD, 0, 52, NULL },
		{ "rArmWidth", TXTFIELD_BYTE, 0, 56, NULL },
		{ "rArmHeight", TXTFIELD_BYTE, 0, 57, NULL },
		{ "torsoLeft", TXTFIELD_DWORD, 0, 60, NULL },
		{ "torsoRight", TXTFIELD_DWORD, 0, 64, NULL },
		{ "torsoTop", TXTFIELD_DWORD, 0, 68, NULL },
		{ "torsoBottom", TXTFIELD_DWORD, 0, 72, NULL },
		{ "torsoWidth", TXTFIELD_BYTE, 0, 76, NULL },
		{ "torsoHeight", TXTFIELD_BYTE, 0, 77, NULL },
		{ "lArmLeft", TXTFIELD_DWORD, 0, 80, NULL },
		{ "lArmRight", TXTFIELD_DWORD, 0, 84, NULL },
		{ "lArmTop", TXTFIELD_DWORD, 0, 88, NULL },
		{ "lArmBottom", TXTFIELD_DWORD, 0, 92, NULL },
		{ "lArmWidth", TXTFIELD_BYTE, 0, 96, NULL },
		{ "lArmHeight", TXTFIELD_BYTE, 0, 97, NULL },
		{ "headLeft", TXTFIELD_DWORD, 0, 100, NULL },
		{ "headRight", TXTFIELD_DWORD, 0, 104, NULL },
		{ "headTop", TXTFIELD_DWORD, 0, 108, NULL },
		{ "headBottom", TXTFIELD_DWORD, 0, 112, NULL },
		{ "headWidth", TXTFIELD_BYTE, 0, 116, NULL },
		{ "headHeight", TXTFIELD_BYTE, 0, 117, NULL },
		{ "neckLeft", TXTFIELD_DWORD, 0, 120, NULL },
		{ "neckRight", TXTFIELD_DWORD, 0, 124, NULL },
		{ "neckTop", TXTFIELD_DWORD, 0, 128, NULL },
		{ "neckBottom", TXTFIELD_DWORD, 0, 132, NULL },
		{ "neckWidth", TXTFIELD_BYTE, 0, 136, NULL },
		{ "neckHeight", TXTFIELD_BYTE, 0, 137, NULL },
		{ "rHandLeft", TXTFIELD_DWORD, 0, 140, NULL },
		{ "rHandRight", TXTFIELD_DWORD, 0, 144, NULL },
		{ "rHandTop", TXTFIELD_DWORD, 0, 148, NULL },
		{ "rHandBottom", TXTFIELD_DWORD, 0, 152, NULL },
		{ "rHandWidth", TXTFIELD_BYTE, 0, 156, NULL },
		{ "rHandHeight", TXTFIELD_BYTE, 0, 157, NULL },
		{ "lHandLeft", TXTFIELD_DWORD, 0, 160, NULL },
		{ "lHandRight", TXTFIELD_DWORD, 0, 164, NULL },
		{ "lHandTop", TXTFIELD_DWORD, 0, 168, NULL },
		{ "lHandBottom", TXTFIELD_DWORD, 0, 172, NULL },
		{ "lHandWidth", TXTFIELD_BYTE, 0, 176, NULL },
		{ "lHandHeight", TXTFIELD_BYTE, 0, 177, NULL },
		{ "beltLeft", TXTFIELD_DWORD, 0, 180, NULL },
		{ "beltRight", TXTFIELD_DWORD, 0, 184, NULL },
		{ "beltTop", TXTFIELD_DWORD, 0, 188, NULL },
		{ "beltBottom", TXTFIELD_DWORD, 0, 192, NULL },
		{ "beltWidth", TXTFIELD_BYTE, 0, 196, NULL },
		{ "beltHeight", TXTFIELD_BYTE, 0, 197, NULL },
		{ "feetLeft", TXTFIELD_DWORD, 0, 200, NULL },
		{ "feetRight", TXTFIELD_DWORD, 0, 204, NULL },
		{ "feetTop", TXTFIELD_DWORD, 0, 208, NULL },
		{ "feetBottom", TXTFIELD_DWORD, 0, 212, NULL },
		{ "feetWidth", TXTFIELD_BYTE, 0, 216, NULL },
		{ "feetHeight", TXTFIELD_BYTE, 0, 217, NULL },
		{ "glovesLeft", TXTFIELD_DWORD, 0, 220, NULL },
		{ "glovesRight", TXTFIELD_DWORD, 0, 224, NULL },
		{ "glovesTop", TXTFIELD_DWORD, 0, 228, NULL },
		{ "glovesBottom", TXTFIELD_DWORD, 0, 232, NULL },
		{ "glovesWidth", TXTFIELD_BYTE, 0, 236, NULL },
		{ "glovesHeight", TXTFIELD_BYTE, 0, 237, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pInventoryTxt = (D2InventoryTxt*)DATATBLS_CompileTxt(pMemPool, "inventory", pTbl, &gpDataTables.nInventoryTxtRecordCount, sizeof(D2InventoryTxt));
#define NUM_INVENTORY_PAGE_STATS 16
	D2_ASSERT(gpDataTables.nInventoryTxtRecordCount == NUM_INVENTORY_PAGE_STATS * NUM_GAME_RESOLUTIONS);
}

//D2Common.0x6FD54F10
void __fastcall DATATBLS_UnloadInventoryTxt()
{
	DATATBLS_UnloadBin(gpDataTables.pInventoryTxt);
}

//D2Common.0x6FD54F20 (#10635)
void __stdcall DATATBLS_GetInventoryRect(int nInventoryTxtId, int bHigherRes, D2InvRectStrc* pInvRect)
{

	D2_ASSERT(gpDataTables.pInventoryTxt);

	const int nIndex = nInventoryTxtId + 16 * bHigherRes;
	D2_ASSERT(nIndex < gpDataTables.nInventoryTxtRecordCount);
	const D2InventoryTxt* pInventoryTxtRecord = &gpDataTables.pInventoryTxt[nIndex];
	D2_ASSERT(pInventoryTxtRecord);

	pInvRect->nLeft = pInventoryTxtRecord->pRect.nLeft;
	pInvRect->nRight = pInventoryTxtRecord->pRect.nRight;
	pInvRect->nTop = pInventoryTxtRecord->pRect.nTop;
	pInvRect->nBottom = pInventoryTxtRecord->pRect.nBottom;
}

//D2Common.0x6FD54FB0 (#10636)
void __stdcall DATATBLS_GetInventoryGridInfo(int nInventoryTxtId, int bHigherRes, D2InventoryGridInfoStrc* pInventoryGridInfo)
{
	D2InventoryTxt* pInventoryTxtRecord = NULL;
	int nIndex = nInventoryTxtId + 16 * bHigherRes;

	D2_ASSERT(gpDataTables.pInventoryTxt);
	D2_ASSERT(nIndex < gpDataTables.nInventoryTxtRecordCount);
	pInventoryTxtRecord = &gpDataTables.pInventoryTxt[nIndex];
	D2_ASSERT(pInventoryTxtRecord);

	memcpy(pInventoryGridInfo, &pInventoryTxtRecord->pGridInfo, sizeof(D2InventoryGridInfoStrc));
}

//D2Common.0x6FD55030 (#10637)
void __stdcall DATATBLS_GetInventoryComponentGrid(int nInventoryTxtId, int bHigherRes, D2InvCompGridStrc* pInvCompGrid, int nComponent)
{
	int nIndex = nInventoryTxtId + 16 * bHigherRes;

	D2_ASSERT(gpDataTables.pInventoryTxt);
	D2_ASSERT(nIndex < gpDataTables.nInventoryTxtRecordCount);

	const D2InventoryTxt* pInventoryTxtRecord = &gpDataTables.pInventoryTxt[nIndex];
	D2_ASSERT(pInventoryTxtRecord);

	const D2InvCompGridStrc* ptBodyStats = &pInventoryTxtRecord->pComponents[nComponent];
	D2_ASSERT(ptBodyStats);

	memcpy(pInvCompGrid, ptBodyStats, sizeof(D2InvCompGridStrc));
}

//D2Common.0x6FD550E0
int __fastcall DATATBLS_MapItemsTxtKeywordToNumber(char* szKey)
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
	else if (!_strnicmp(szKey, "stat", 32))
	{
		return 3;
	}
	else
	{
		return -1;
	}
}

//D2Common.0x6FD55140
int __fastcall DATATBLS_Return2()
{
	return 2;
}

//D2Common.0x6FD55150
//TODO: Find a name
int __fastcall sub_6FD55150(char* szText, int* a2, int a3, int nKeywordNumber)
{
	int nRow = 0;

	if (a3 == 1 && nKeywordNumber == 3)
	{
		if (gpDataTables.pItemStatCostLinker)
		{
			nRow = FOG_GetRowFromTxt(gpDataTables.pItemStatCostLinker, szText, 0);
			if (nRow >= 0)
			{
				*a2 = 1;
				return nRow;
			}
		}

		if (!_strnicmp(szText, "base", 32))
		{
			*a2 = 0;
			return 1;
		}

		if (!_strnicmp(szText, "mod", 32))
		{
			*a2 = 0;
			return 2;
		}

		_strnicmp(szText, "accr", 32);//Seems unused / unneeded
		*a2 = 0;
	}

	return 0;
}

//D2Common.0x6FD551E0
void __fastcall DATATBLS_ItemCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	int nBufferSize = 0;
	char pBuffer[1024] = {};

	if (pRecord)
	{
		if (pSrc)
		{
			nBufferSize = FOG_10254(pSrc, pBuffer, sizeof(pBuffer), DATATBLS_MapItemsTxtKeywordToNumber, DATATBLS_Return2, sub_6FD55150);
			if (nBufferSize > 0)
			{
				*(DWORD*)((char*)pRecord + nOffset) = DATATBLS_AppendMemoryBuffer(&gpDataTables.pItemsCode, (int*)&gpDataTables.nItemsCodeSize, &gpDataTables.nItemsCodeSizeEx, pBuffer, nBufferSize);
			}
			else
			{
				*(DWORD*)((char*)pRecord + nOffset) = -1;
			}
		}
		else
		{
			*(DWORD*)((char*)pRecord + nOffset) = -1;
		}
	}
}

//D2Common.0x6FD55280
void __fastcall DATATBLS_LoadItemsTxt(void* pMemPool)
{
	D2ItemsTxt* pWeapons = NULL;
	D2ItemsTxt* pArmor = NULL;
	D2ItemsTxt* pMisc = NULL;
	D2ItemsTxt* pItems = NULL;
	int nOldCounter = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 128, &gpDataTables.pItemsLinker },
		{ "namestr", TXTFIELD_KEYTOWORD, 0, 244, DATATBLS_GetStringIdFromReferenceString },
		{ "version", TXTFIELD_WORD, 0, 246, NULL },
		{ "compactsave", TXTFIELD_UNKNOWN2, 0, 323, NULL },
		{ "type", TXTFIELD_CODETOWORD, 0, 286, &gpDataTables.pItemTypesLinker },
		{ "type2", TXTFIELD_CODETOWORD, 0, 288, &gpDataTables.pItemTypesLinker },
		{ "hit class", TXTFIELD_CODETOBYTE, 0, 316, &gpDataTables.pHitClassLinker },
		{ "gemapplytype", TXTFIELD_BYTE, 0, 318, NULL },
		{ "levelreq", TXTFIELD_BYTE, 0, 319, NULL },
		{ "magic lvl", TXTFIELD_BYTE, 0, 320, NULL },
		{ "auto prefix", TXTFIELD_WORD, 0, 248, NULL },
		{ "gamble cost", TXTFIELD_DWORD, 0, 212, NULL },
		{ "alternategfx", TXTFIELD_RAW, 0, 144, NULL },
		{ "spawnable", TXTFIELD_BYTE, 0, 307, NULL },
		{ "rarity", TXTFIELD_BYTE, 0, 252, NULL },
		{ "level", TXTFIELD_BYTE, 0, 253, NULL },
		{ "mindam", TXTFIELD_UNKNOWN2, 0, 254, NULL },
		{ "maxdam", TXTFIELD_UNKNOWN2, 0, 255, NULL },
		{ "2handmindam", TXTFIELD_UNKNOWN2, 0, 258, NULL },
		{ "2handmaxdam", TXTFIELD_UNKNOWN2, 0, 259, NULL },
		{ "minmisdam", TXTFIELD_BYTE, 0, 256, NULL },
		{ "maxmisdam", TXTFIELD_BYTE, 0, 257, NULL },
		{ "minac", TXTFIELD_DWORD, 0, 204, NULL },
		{ "maxac", TXTFIELD_DWORD, 0, 208, NULL },
		{ "block", TXTFIELD_UNKNOWN2, 0, 273, NULL },
		{ "absorbs", TXTFIELD_UNKNOWN2, 0, 270, NULL },
		{ "rangeadder", TXTFIELD_UNKNOWN2, 0, 260, NULL },
		{ "speed", TXTFIELD_DWORD, 0, 216, NULL },
		{ "strbonus", TXTFIELD_WORD, 0, 262, NULL },
		{ "dexbonus", TXTFIELD_WORD, 0, 264, NULL },
		{ "reqstr", TXTFIELD_WORD, 0, 266, NULL },
		{ "reqdex", TXTFIELD_WORD, 0, 268, NULL },
		{ "invwidth", TXTFIELD_BYTE, 0, 271, NULL },
		{ "invheight", TXTFIELD_BYTE, 0, 272, NULL },
		{ "stackable", TXTFIELD_BYTE, 0, 306, NULL },
		{ "minstack", TXTFIELD_DWORD, 0, 228, NULL },
		{ "maxstack", TXTFIELD_DWORD, 0, 232, NULL },
		{ "spawnstack", TXTFIELD_DWORD, 0, 236, NULL },
		{ "flippyfile", TXTFIELD_ASCII, 31, 0, NULL },
		{ "invfile", TXTFIELD_ASCII, 31, 32, NULL },
		{ "uniqueinvfile", TXTFIELD_ASCII, 31, 64, NULL },
		{ "setinvfile", TXTFIELD_ASCII, 31, 96, NULL },
		{ "durability", TXTFIELD_UNKNOWN2, 0, 274, NULL },
		{ "nodurability", TXTFIELD_BYTE, 0, 275, NULL },
		{ "missile", TXTFIELD_UNKNOWN2, 0, 276, NULL },
		{ "component", TXTFIELD_BYTE, 0, 277, NULL },
		{ "cost", TXTFIELD_DWORD, 0, 224, NULL },
		{ "normcode", TXTFIELD_RAW, 0, 132, NULL },
		{ "ubercode", TXTFIELD_RAW, 0, 136, NULL },
		{ "ultracode", TXTFIELD_RAW, 0, 140, NULL },
		{ "wclass", TXTFIELD_RAW, 0, 192, NULL },
		{ "2handedwclass", TXTFIELD_RAW, 0, 196, NULL },
		{ "1or2handed", TXTFIELD_UNKNOWN2, 0, 317, NULL },
		{ "BetterGem", TXTFIELD_RAW, 0, 188, NULL },
		{ "rArm", TXTFIELD_UNKNOWN2, 0, 278, NULL },
		{ "lArm", TXTFIELD_UNKNOWN2, 0, 279, NULL },
		{ "torso", TXTFIELD_UNKNOWN2, 0, 280, NULL },
		{ "legs", TXTFIELD_UNKNOWN2, 0, 281, NULL },
		{ "rspad", TXTFIELD_UNKNOWN2, 0, 282, NULL },
		{ "lspad", TXTFIELD_UNKNOWN2, 0, 283, NULL },
		{ "2handed", TXTFIELD_UNKNOWN2, 0, 284, NULL },
		{ "useable", TXTFIELD_BYTE, 0, 285, NULL },
		{ "subtype", TXTFIELD_UNKNOWN2, 0, 290, NULL },
		{ "dropsound", TXTFIELD_NAMETOWORD, 0, 292, &gpDataTables.pSoundsLinker },
		{ "dropsfxframe", TXTFIELD_BYTE, 0, 296, NULL },
		{ "usesound", TXTFIELD_NAMETOWORD, 0, 294, &gpDataTables.pSoundsLinker },
		{ "unique", TXTFIELD_BYTE, 0, 297, NULL },
		{ "quest", TXTFIELD_BYTE, 0, 298, NULL },
		{ "questdiffcheck", TXTFIELD_BYTE, 0, 299, NULL },
		{ "transparent", TXTFIELD_BYTE, 0, 300, NULL },
		{ "transtbl", TXTFIELD_BYTE, 0, 301, NULL },
		{ "lightradius", TXTFIELD_BYTE, 0, 303, NULL },
		{ "belt", TXTFIELD_BYTE, 0, 304, NULL },
		{ "autobelt", TXTFIELD_BYTE, 0, 305, NULL },
		{ "missiletype", TXTFIELD_WORD, 0, 250, NULL },
		{ "spellicon", TXTFIELD_UNKNOWN2, 0, 308, NULL },
		{ "durwarning", TXTFIELD_BYTE, 0, 309, NULL },
		{ "qntwarning", TXTFIELD_BYTE, 0, 310, NULL },
		{ "hasinv", TXTFIELD_UNKNOWN2, 0, 311, NULL },
		{ "gemsockets", TXTFIELD_UNKNOWN2, 0, 312, NULL },
		{ "gemoffset", TXTFIELD_DWORD, 0, 240, NULL },
		{ "pSpell", TXTFIELD_DWORD, 0, 148, NULL },
		{ "state", TXTFIELD_NAMETOWORD, 0, 152, &gpDataTables.pStatesLinker },
		{ "cstate1", TXTFIELD_NAMETOWORD, 0, 154, &gpDataTables.pStatesLinker },
		{ "cstate2", TXTFIELD_NAMETOWORD, 0, 156, &gpDataTables.pStatesLinker },
		{ "len", TXTFIELD_CALCTODWORD, 0, 176, DATATBLS_ItemCalcLinker },
		{ "stat1", TXTFIELD_NAMETOWORD, 0, 158, &gpDataTables.pItemStatCostLinker },
		{ "calc1", TXTFIELD_CALCTODWORD, 0, 164, DATATBLS_ItemCalcLinker },
		{ "stat2", TXTFIELD_NAMETOWORD, 0, 160, &gpDataTables.pItemStatCostLinker },
		{ "calc2", TXTFIELD_CALCTODWORD, 0, 168, DATATBLS_ItemCalcLinker },
		{ "stat3", TXTFIELD_NAMETOWORD, 0, 162, &gpDataTables.pItemStatCostLinker },
		{ "calc3", TXTFIELD_CALCTODWORD, 0, 172, DATATBLS_ItemCalcLinker },
		{ "spelldesc", TXTFIELD_BYTE, 0, 180, NULL },
		{ "spelldescstr", TXTFIELD_KEYTOWORD, 0, 182, DATATBLS_GetStringIdFromReferenceString },
		{ "spelldesccalc", TXTFIELD_CALCTODWORD, 0, 184, DATATBLS_ItemCalcLinker },
		{ "SkipName", TXTFIELD_BYTE, 0, 324, NULL },
		{ "bitfield1", TXTFIELD_DWORD, 0, 220, NULL },
		{ "CharsiMin", TXTFIELD_BYTE, 0, 328, NULL },
		{ "CharsiMax", TXTFIELD_BYTE, 0, 345, NULL },
		{ "CharsiMagicMin", TXTFIELD_BYTE, 0, 362, NULL },
		{ "CharsiMagicMax", TXTFIELD_BYTE, 0, 379, NULL },
		{ "CharsiMagicLvl", TXTFIELD_BYTE, 0, 396, NULL },
		{ "GheedMin", TXTFIELD_BYTE, 0, 327, NULL },
		{ "GheedMax", TXTFIELD_BYTE, 0, 344, NULL },
		{ "GheedMagicMin", TXTFIELD_BYTE, 0, 361, NULL },
		{ "GheedMagicMax", TXTFIELD_BYTE, 0, 378, NULL },
		{ "GheedMagicLvl", TXTFIELD_BYTE, 0, 395, NULL },
		{ "AkaraMin", TXTFIELD_BYTE, 0, 326, NULL },
		{ "AkaraMax", TXTFIELD_BYTE, 0, 343, NULL },
		{ "AkaraMagicMin", TXTFIELD_BYTE, 0, 360, NULL },
		{ "AkaraMagicMax", TXTFIELD_BYTE, 0, 377, NULL },
		{ "AkaraMagicLvl", TXTFIELD_BYTE, 0, 394, NULL },
		{ "FaraMin", TXTFIELD_BYTE, 0, 329, NULL },
		{ "FaraMax", TXTFIELD_BYTE, 0, 346, NULL },
		{ "FaraMagicMin", TXTFIELD_BYTE, 0, 363, NULL },
		{ "FaraMagicMax", TXTFIELD_BYTE, 0, 380, NULL },
		{ "FaraMagicLvl", TXTFIELD_BYTE, 0, 397, NULL },
		{ "LysanderMin", TXTFIELD_BYTE, 0, 330, NULL },
		{ "LysanderMax", TXTFIELD_BYTE, 0, 347, NULL },
		{ "LysanderMagicMin", TXTFIELD_BYTE, 0, 364, NULL },
		{ "LysanderMagicMax", TXTFIELD_BYTE, 0, 381, NULL },
		{ "LysanderMagicLvl", TXTFIELD_BYTE, 0, 398, NULL },
		{ "DrognanMin", TXTFIELD_BYTE, 0, 331, NULL },
		{ "DrognanMax", TXTFIELD_BYTE, 0, 348, NULL },
		{ "DrognanMagicMin", TXTFIELD_BYTE, 0, 365, NULL },
		{ "DrognanMagicMax", TXTFIELD_BYTE, 0, 382, NULL },
		{ "DrognanMagicLvl", TXTFIELD_BYTE, 0, 399, NULL },
		{ "HraltiMin", TXTFIELD_BYTE, 0, 332, NULL },
		{ "HraltiMax", TXTFIELD_BYTE, 0, 349, NULL },
		{ "HraltiMagicMin", TXTFIELD_BYTE, 0, 366, NULL },
		{ "HraltiMagicMax", TXTFIELD_BYTE, 0, 383, NULL },
		{ "HraltiMagicLvl", TXTFIELD_BYTE, 0, 400, NULL },
		{ "AlkorMin", TXTFIELD_BYTE, 0, 333, NULL },
		{ "AlkorMax", TXTFIELD_BYTE, 0, 350, NULL },
		{ "AlkorMagicMin", TXTFIELD_BYTE, 0, 367, NULL },
		{ "AlkorMagicMax", TXTFIELD_BYTE, 0, 384, NULL },
		{ "AlkorMagicLvl", TXTFIELD_BYTE, 0, 401, NULL },
		{ "OrmusMin", TXTFIELD_BYTE, 0, 334, NULL },
		{ "OrmusMax", TXTFIELD_BYTE, 0, 351, NULL },
		{ "OrmusMagicMin", TXTFIELD_BYTE, 0, 368, NULL },
		{ "OrmusMagicMax", TXTFIELD_BYTE, 0, 385, NULL },
		{ "OrmusMagicLvl", TXTFIELD_BYTE, 0, 402, NULL },
		{ "ElzixMin", TXTFIELD_BYTE, 0, 335, NULL },
		{ "ElzixMax", TXTFIELD_BYTE, 0, 352, NULL },
		{ "ElzixMagicMin", TXTFIELD_BYTE, 0, 369, NULL },
		{ "ElzixMagicMax", TXTFIELD_BYTE, 0, 386, NULL },
		{ "ElzixMagicLvl", TXTFIELD_BYTE, 0, 403, NULL },
		{ "AshearaMin", TXTFIELD_BYTE, 0, 336, NULL },
		{ "AshearaMax", TXTFIELD_BYTE, 0, 353, NULL },
		{ "AshearaMagicMin", TXTFIELD_BYTE, 0, 370, NULL },
		{ "AshearaMagicMax", TXTFIELD_BYTE, 0, 387, NULL },
		{ "AshearaMagicLvl", TXTFIELD_BYTE, 0, 404, NULL },
		{ "CainMin", TXTFIELD_BYTE, 0, 337, NULL },
		{ "CainMax", TXTFIELD_BYTE, 0, 354, NULL },
		{ "CainMagicMin", TXTFIELD_BYTE, 0, 371, NULL },
		{ "CainMagicMax", TXTFIELD_BYTE, 0, 388, NULL },
		{ "CainMagicLvl", TXTFIELD_BYTE, 0, 405, NULL },
		{ "HalbuMin", TXTFIELD_BYTE, 0, 338, NULL },
		{ "HalbuMax", TXTFIELD_BYTE, 0, 355, NULL },
		{ "HalbuMagicMin", TXTFIELD_BYTE, 0, 372, NULL },
		{ "HalbuMagicMax", TXTFIELD_BYTE, 0, 389, NULL },
		{ "HalbuMagicLvl", TXTFIELD_BYTE, 0, 406, NULL },
		{ "JamellaMin", TXTFIELD_BYTE, 0, 339, NULL },
		{ "JamellaMax", TXTFIELD_BYTE, 0, 356, NULL },
		{ "JamellaMagicMin", TXTFIELD_BYTE, 0, 373, NULL },
		{ "JamellaMagicMax", TXTFIELD_BYTE, 0, 390, NULL },
		{ "JamellaMagicLvl", TXTFIELD_BYTE, 0, 407, NULL },
		{ "MalahMin", TXTFIELD_BYTE, 0, 340, NULL },
		{ "MalahMax", TXTFIELD_BYTE, 0, 357, NULL },
		{ "MalahMagicMin", TXTFIELD_BYTE, 0, 374, NULL },
		{ "MalahMagicMax", TXTFIELD_BYTE, 0, 391, NULL },
		{ "MalahMagicLvl", TXTFIELD_BYTE, 0, 408, NULL },
		{ "LarzukMin", TXTFIELD_BYTE, 0, 341, NULL },
		{ "LarzukMax", TXTFIELD_BYTE, 0, 358, NULL },
		{ "LarzukMagicMin", TXTFIELD_BYTE, 0, 375, NULL },
		{ "LarzukMagicMax", TXTFIELD_BYTE, 0, 392, NULL },
		{ "LarzukMagicLvl", TXTFIELD_BYTE, 0, 409, NULL },
		{ "DrehyaMin", TXTFIELD_BYTE, 0, 342, NULL },
		{ "DrehyaMax", TXTFIELD_BYTE, 0, 359, NULL },
		{ "DrehyaMagicMin", TXTFIELD_BYTE, 0, 376, NULL },
		{ "DrehyaMagicMax", TXTFIELD_BYTE, 0, 393, NULL },
		{ "DrehyaMagicLvl", TXTFIELD_BYTE, 0, 410, NULL },
		{ "Transmogrify", TXTFIELD_UNKNOWN2, 0, 313, NULL },
		{ "TMogType", TXTFIELD_RAW, 0, 200, NULL },
		{ "TMogMin", TXTFIELD_UNKNOWN2, 0, 314, NULL },
		{ "TMogMax", TXTFIELD_UNKNOWN2, 0, 315, NULL },
		{ "Transform", TXTFIELD_BYTE, 0, 321, NULL },
		{ "InvTrans", TXTFIELD_BYTE, 0, 322, NULL },
		{ "PermStoreItem", TXTFIELD_BYTE, 0, 420, NULL },
		{ "multibuy", TXTFIELD_BYTE, 0, 421, NULL },
		{ "NightmareUpgrade", TXTFIELD_RAW, 0, 412, NULL },
		{ "HellUpgrade", TXTFIELD_RAW, 0, 416, NULL },
		{ "Nameable", TXTFIELD_BYTE, 0, 325, NULL },
		{ "end", 0, 0, 0, NULL },
	};


	gpDataTables.pItemsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	pWeapons = (D2ItemsTxt*)DATATBLS_CompileTxt(pMemPool, "weapons", pTbl, &gpDataTables.pItemDataTables.nWeaponsTxtRecordCount, sizeof(D2ItemsTxt));
	pArmor = (D2ItemsTxt*)DATATBLS_CompileTxt(pMemPool, "armor", pTbl, &gpDataTables.pItemDataTables.nArmorTxtRecordCount, sizeof(D2ItemsTxt));
	pMisc = (D2ItemsTxt*)DATATBLS_CompileTxt(pMemPool, "misc", pTbl, &gpDataTables.pItemDataTables.nMiscTxtRecordCount, sizeof(D2ItemsTxt));

	gpDataTables.pItemDataTables.nItemsTxtRecordCount = gpDataTables.pItemDataTables.nWeaponsTxtRecordCount + gpDataTables.pItemDataTables.nMiscTxtRecordCount + gpDataTables.pItemDataTables.nArmorTxtRecordCount;
	pItems = (D2ItemsTxt*)FOG_AllocServerMemory(NULL, sizeof(D2ItemsTxt) * gpDataTables.pItemDataTables.nItemsTxtRecordCount, __FILE__, __LINE__, 0);

	gpDataTables.pItemDataTables.pItemsTxt = pItems;

	gpDataTables.pItemDataTables.pWeapons = pItems;
	gpDataTables.pItemDataTables.pArmor = &gpDataTables.pItemDataTables.pWeapons[gpDataTables.pItemDataTables.nWeaponsTxtRecordCount];
	gpDataTables.pItemDataTables.pMisc = &gpDataTables.pItemDataTables.pArmor[gpDataTables.pItemDataTables.nArmorTxtRecordCount];

	memcpy(gpDataTables.pItemDataTables.pWeapons, pWeapons, sizeof(D2ItemsTxt) * gpDataTables.pItemDataTables.nWeaponsTxtRecordCount);
	memcpy(gpDataTables.pItemDataTables.pArmor, pArmor, sizeof(D2ItemsTxt) * gpDataTables.pItemDataTables.nArmorTxtRecordCount);
	memcpy(gpDataTables.pItemDataTables.pMisc, pMisc, sizeof(D2ItemsTxt) * gpDataTables.pItemDataTables.nMiscTxtRecordCount);

	DATATBLS_UnloadBin(pWeapons);
	DATATBLS_UnloadBin(pArmor);
	DATATBLS_UnloadBin(pMisc);

	DATATBLS_GetBinFileHandle(pMemPool, "itemscode", (void**)&gpDataTables.pItemsCode, (int*)&gpDataTables.nItemsCodeSize, &gpDataTables.nItemsCodeSizeEx);

	if (!gpDataTables.bCompileTxt)
	{
		for (int i = 0; i < gpDataTables.pItemDataTables.nItemsTxtRecordCount; ++i)
		{
			FOG_10215(gpDataTables.pItemsLinker, gpDataTables.pItemDataTables.pItemsTxt[i].dwCode);
		}
	}

	gpDataTables.pIndexOldToCurrent = (WORD*)FOG_AllocServerMemory(NULL, sizeof(WORD) * gpDataTables.pItemDataTables.nItemsTxtRecordCount, __FILE__, __LINE__, 0);
	memset(gpDataTables.pIndexOldToCurrent, 0x00, sizeof(WORD) * gpDataTables.pItemDataTables.nItemsTxtRecordCount);

	nOldCounter = 0;
	for (int i = 0; i < gpDataTables.pItemDataTables.nItemsTxtRecordCount; ++i)
	{
		if (!gpDataTables.pItemDataTables.pItemsTxt[i].wVersion)
		{
			gpDataTables.pIndexOldToCurrent[nOldCounter] = i;
			++nOldCounter;
		}
	}
}

//D2Common.0x6FD575D0
void __fastcall DATATBLS_UnloadItemsTxt()
{
	FOG_FreeLinker(gpDataTables.pItemsLinker);

	if (gpDataTables.pItemDataTables.pItemsTxt)
	{
		FOG_FreeServerMemory(NULL, gpDataTables.pItemDataTables.pItemsTxt, __FILE__, __LINE__, 0);
	}

	if (gpDataTables.pIndexOldToCurrent)
	{
		FOG_FreeServerMemory(NULL, gpDataTables.pIndexOldToCurrent, __FILE__, __LINE__, 0);
	}

	gpDataTables.pItemDataTables.pItemsTxt = NULL;
}

//D2Common.0x6FD57620 (#10599)
D2ItemDataTbl* __stdcall DATATBLS_GetItemDataTables()
{
	return &gpDataTables.pItemDataTables;
}

//D2Common.0x6FD57630 (#10597)
int __stdcall DATATBLS_MapOldItemIndexToCurrent(int nItemId)
{
	if (nItemId < gpDataTables.pItemDataTables.nItemsTxtRecordCount)
	{
		if (nItemId >= 0)
		{
			D2_ASSERT(gpDataTables.pIndexOldToCurrent);
			return gpDataTables.pIndexOldToCurrent[nItemId];
		}
	}

	return -1;
}

//D2Common.0x6FD57680 (#10600)
D2ItemsTxt* __stdcall DATATBLS_GetItemsTxtRecord(int nItemId)
{
	if (nItemId < gpDataTables.pItemDataTables.nItemsTxtRecordCount)
	{
		D2_ASSERT(gpDataTables.pItemDataTables.pItemsTxt);
		return &gpDataTables.pItemDataTables.pItemsTxt[nItemId];
	}

	return NULL;
}

//D2Common.0x6FD576D0 (#10601)
D2ItemsTxt* __stdcall DATATBLS_GetItemRecordFromItemCode(DWORD dwCode, int* pItemId)
{
	*pItemId = FOG_GetLinkIndex(gpDataTables.pItemsLinker, dwCode, 0);
	if (*pItemId >= 0)
	{
		return &gpDataTables.pItemDataTables.pItemsTxt[*pItemId];
	}
	
	*pItemId = 0;
	return NULL;
}

//D2Common.0x6FD57720 (#10602)
int __stdcall DATATBLS_GetItemIdFromItemCode(DWORD dwCode)
{
	return FOG_GetLinkIndex(gpDataTables.pItemsLinker, dwCode, 0);
}

//D2Common.0x6FD57740
void __fastcall DATATBLS_ItemParamLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	int nRow = 0;

	if (pRecord)
	{
		if (pSrc && *pSrc != 0)
		{
			if (*pSrc == '-' || *pSrc >= '0' && *pSrc <= '9')
			{
				*(DWORD*)((char*)pRecord + nOffset) = atoi(pSrc);
			}
			else
			{
				if (gpDataTables.pSkillsLinker)
				{
					nRow = FOG_GetRowFromTxt(gpDataTables.pSkillsLinker, pSrc, 0);
					if (nRow >= 0)
					{
						*(DWORD*)((char*)pRecord + nOffset) = nRow;
						return;
					}
				}

				if (gpDataTables.pMonTypeLinker)
				{
					nRow = FOG_GetRowFromTxt(gpDataTables.pMonTypeLinker, pSrc, 0);
					if (nRow >= 0)
					{
						*(DWORD*)((char*)pRecord + nOffset) = nRow;
						return;
					}
				}

				if (gpDataTables.pStatesLinker)
				{
					nRow = FOG_GetRowFromTxt(gpDataTables.pStatesLinker, pSrc, 0);
					if (nRow >= 0)
					{
						*(DWORD*)((char*)pRecord + nOffset) = nRow;
						return;
					}
				}

				*(DWORD*)((char*)pRecord + nOffset) = 0;
				FOG_WriteToLogFile("Failed to parse '%s' line %d", pSrc, nTxtRow);
			}
		}
		else
		{
			*(DWORD*)((char*)pRecord + nOffset) = 0;
		}
	}
}

//D2Common.0x6FD57820
void __fastcall DATATBLS_LoadMagicSuffix_Prefix_AutomagicTxt(void* pMemPool)
{
	wchar_t* pUnicode = NULL;

	int nSuffixRecords = 0;
	int nPrefixRecords = 0;
	int nAutoMagicRecords = 0;

	D2MagicAffixTxt* pMagicSuffix = NULL;
	D2MagicAffixTxt* pMagicPrefix = NULL;
	D2MagicAffixTxt* pAutoMagic = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_ASCII, 31, 0, NULL },
		{ "spawnable", TXTFIELD_BYTE, 0, 84, NULL },
		{ "level", TXTFIELD_DWORD, 0, 88, NULL },
		{ "group", TXTFIELD_DWORD, 0, 92, NULL },
		{ "version", TXTFIELD_WORD, 0, 34, NULL },
		{ "divide", TXTFIELD_DWORD, 0, 132, NULL },
		{ "multiply", TXTFIELD_DWORD, 0, 136, NULL },
		{ "add", TXTFIELD_DWORD, 0, 140, NULL },
		{ "rare", TXTFIELD_BYTE, 0, 100, NULL },
		{ "maxlevel", TXTFIELD_DWORD, 0, 96, NULL },
		{ "levelreq", TXTFIELD_BYTE, 0, 101, NULL },
		{ "classspecific", TXTFIELD_CODETOBYTE, 0, 102, &gpDataTables.pPlayerClassLinker },
		{ "class", TXTFIELD_CODETOBYTE, 0, 103, &gpDataTables.pPlayerClassLinker },
		{ "classlevelreq", TXTFIELD_BYTE, 0, 104, NULL },
		{ "itype1", TXTFIELD_CODETOWORD, 0, 106, &gpDataTables.pItemTypesLinker },
		{ "itype2", TXTFIELD_CODETOWORD, 0, 108, &gpDataTables.pItemTypesLinker },
		{ "itype3", TXTFIELD_CODETOWORD, 0, 110, &gpDataTables.pItemTypesLinker },
		{ "itype4", TXTFIELD_CODETOWORD, 0, 112, &gpDataTables.pItemTypesLinker },
		{ "itype5", TXTFIELD_CODETOWORD, 0, 114, &gpDataTables.pItemTypesLinker },
		{ "itype6", TXTFIELD_CODETOWORD, 0, 116, &gpDataTables.pItemTypesLinker },
		{ "itype7", TXTFIELD_CODETOWORD, 0, 118, &gpDataTables.pItemTypesLinker },
		{ "etype1", TXTFIELD_CODETOWORD, 0, 120, &gpDataTables.pItemTypesLinker },
		{ "etype2", TXTFIELD_CODETOWORD, 0, 122, &gpDataTables.pItemTypesLinker },
		{ "etype3", TXTFIELD_CODETOWORD, 0, 124, &gpDataTables.pItemTypesLinker },
		{ "etype4", TXTFIELD_CODETOWORD, 0, 126, &gpDataTables.pItemTypesLinker },
		{ "etype5", TXTFIELD_CODETOWORD, 0, 128, &gpDataTables.pItemTypesLinker },
		{ "transformcolor", TXTFIELD_CODETOBYTE, 0, 86, &gpDataTables.pColorsLinker },
		{ "frequency", TXTFIELD_BYTE, 0, 130, NULL },
		{ "mod1code", TXTFIELD_NAMETODWORD, 0, 36, &gpDataTables.pPropertiesLinker },
		{ "mod1param", TXTFIELD_CALCTODWORD, 0, 40, DATATBLS_ItemParamLinker },
		{ "mod1min", TXTFIELD_DWORD, 0, 44, NULL },
		{ "mod1max", TXTFIELD_DWORD, 0, 48, NULL },
		{ "mod2code", TXTFIELD_NAMETODWORD, 0, 52, &gpDataTables.pPropertiesLinker },
		{ "mod2param", TXTFIELD_CALCTODWORD, 0, 56, DATATBLS_ItemParamLinker },
		{ "mod2min", TXTFIELD_DWORD, 0, 60, NULL },
		{ "mod2max", TXTFIELD_DWORD, 0, 64, NULL },
		{ "mod3code", TXTFIELD_NAMETODWORD, 0, 68, &gpDataTables.pPropertiesLinker },
		{ "mod3param", TXTFIELD_CALCTODWORD, 0, 72, DATATBLS_ItemParamLinker },
		{ "mod3min", TXTFIELD_DWORD, 0, 76, NULL },
		{ "mod3max", TXTFIELD_DWORD, 0, 80, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	pMagicSuffix = (D2MagicAffixTxt*)DATATBLS_CompileTxt(pMemPool, "magicsuffix", pTbl, &nSuffixRecords, sizeof(D2MagicAffixTxt));
	pMagicPrefix = (D2MagicAffixTxt*)DATATBLS_CompileTxt(pMemPool, "magicprefix", pTbl, &nPrefixRecords, sizeof(D2MagicAffixTxt));
	pAutoMagic = (D2MagicAffixTxt*)DATATBLS_CompileTxt(pMemPool, "automagic", pTbl, &nAutoMagicRecords, sizeof(D2MagicAffixTxt));

	gpDataTables.pMagicAffixDataTables.nMagicAffixTxtRecordCount = nSuffixRecords + nAutoMagicRecords + nPrefixRecords;
	gpDataTables.pMagicAffixDataTables.pMagicAffixTxt = (D2MagicAffixTxt *)FOG_AllocServerMemory(NULL, sizeof(D2MagicAffixTxt) * gpDataTables.pMagicAffixDataTables.nMagicAffixTxtRecordCount, __FILE__, __LINE__, 0);
	gpDataTables.pMagicAffixDataTables.pMagicSuffix = gpDataTables.pMagicAffixDataTables.pMagicAffixTxt;
	memcpy(gpDataTables.pMagicAffixDataTables.pMagicAffixTxt, pMagicSuffix, sizeof(D2MagicAffixTxt) * nSuffixRecords);
	gpDataTables.pMagicAffixDataTables.pMagicPrefix = &gpDataTables.pMagicAffixDataTables.pMagicSuffix[nSuffixRecords];
	memcpy(&gpDataTables.pMagicAffixDataTables.pMagicSuffix[nSuffixRecords], pMagicPrefix, sizeof(D2MagicAffixTxt) * nPrefixRecords);
	gpDataTables.pMagicAffixDataTables.pAutoMagic = &gpDataTables.pMagicAffixDataTables.pMagicPrefix[nPrefixRecords];
	memcpy(&gpDataTables.pMagicAffixDataTables.pMagicPrefix[nPrefixRecords], pAutoMagic, sizeof(D2MagicAffixTxt) * nAutoMagicRecords);

	DATATBLS_UnloadBin(pMagicSuffix);
	DATATBLS_UnloadBin(pMagicPrefix);
	DATATBLS_UnloadBin(pAutoMagic);

	for (int i = 0; i < gpDataTables.pMagicAffixDataTables.nMagicAffixTxtRecordCount; ++i)
	{
		gpDataTables.pMagicAffixDataTables.pMagicAffixTxt[i].wTblIndex = D2LANG_GetTblIndex(gpDataTables.pMagicAffixDataTables.pMagicAffixTxt[i].szName, &pUnicode);
	}
}

//D2Common.0x6FD58080
void __fastcall DATATBLS_UnloadMagicSuffix_Prefix_AutomagicTxt()
{
	if (gpDataTables.pMagicAffixDataTables.pMagicAffixTxt)
	{
		FOG_FreeServerMemory(NULL, gpDataTables.pMagicAffixDataTables.pMagicAffixTxt, __FILE__, __LINE__, 0);
	}
	gpDataTables.pMagicAffixDataTables.pMagicAffixTxt = NULL;
}

//D2Common.0x6FD580B0 (#10603)
D2MagicAffixDataTbl* __stdcall DATATBLS_GetMagicAffixDataTables()
{
	return &gpDataTables.pMagicAffixDataTables;
}

//D2Common.0x6FD580C0 (#10604)
D2MagicAffixTxt* __stdcall DATATBLS_GetMagicAffixTxtRecord(int nIndex)
{
	D2_ASSERT(gpDataTables.pMagicAffixDataTables.pMagicAffixTxt);
	if (nIndex > gpDataTables.pMagicAffixDataTables.nMagicAffixTxtRecordCount || nIndex <= 0)
	{
		return NULL;
	}
	else
	{
		return &gpDataTables.pMagicAffixDataTables.pMagicAffixTxt[nIndex - 1];
	}
}

//D2Common.0x6FD58110
void __fastcall DATATBLS_LoadRareSuffix_PrefixTxt(void* pMemPool)
{
	wchar_t* pUnicode = NULL;

	D2RareAffixTxt* pRareSuffix = NULL;
	D2RareAffixTxt* pRarePrefix = NULL;

	int nSuffixRecords = 0;
	int nPrefixRecords = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_ASCII, 31, 38, NULL },
		{ "version", TXTFIELD_WORD, 0, 14, NULL },
		{ "itype1", TXTFIELD_CODETOWORD, 0, 16, &gpDataTables.pItemTypesLinker },
		{ "itype2", TXTFIELD_CODETOWORD, 0, 18, &gpDataTables.pItemTypesLinker },
		{ "itype3", TXTFIELD_CODETOWORD, 0, 20, &gpDataTables.pItemTypesLinker },
		{ "itype4", TXTFIELD_CODETOWORD, 0, 22, &gpDataTables.pItemTypesLinker },
		{ "itype5", TXTFIELD_CODETOWORD, 0, 24, &gpDataTables.pItemTypesLinker },
		{ "itype6", TXTFIELD_CODETOWORD, 0, 26, &gpDataTables.pItemTypesLinker },
		{ "itype7", TXTFIELD_CODETOWORD, 0, 28, &gpDataTables.pItemTypesLinker },
		{ "etype1", TXTFIELD_CODETOWORD, 0, 30, &gpDataTables.pItemTypesLinker },
		{ "etype2", TXTFIELD_CODETOWORD, 0, 32, &gpDataTables.pItemTypesLinker },
		{ "etype3", TXTFIELD_CODETOWORD, 0, 34, &gpDataTables.pItemTypesLinker },
		{ "etype4", TXTFIELD_CODETOWORD, 0, 36, &gpDataTables.pItemTypesLinker },
		{ "end", 0, 0, 0, NULL },
	};

	pRareSuffix = (D2RareAffixTxt*)DATATBLS_CompileTxt(pMemPool, "raresuffix", pTbl, &nSuffixRecords, sizeof(D2RareAffixTxt));
	pRarePrefix = (D2RareAffixTxt*)DATATBLS_CompileTxt(pMemPool, "rareprefix", pTbl, &nPrefixRecords, sizeof(D2RareAffixTxt));

	gpDataTables.pRareAffixDataTables.nRareAffixTxtRecordCount = nPrefixRecords + nSuffixRecords;

	gpDataTables.pRareAffixDataTables.pRareAffixTxt = (D2RareAffixTxt*)FOG_AllocServerMemory(NULL, sizeof(D2RareAffixTxt) * (nPrefixRecords + nSuffixRecords), __FILE__, __LINE__, 0);

	gpDataTables.pRareAffixDataTables.pRareSuffix = gpDataTables.pRareAffixDataTables.pRareAffixTxt;
	memcpy(gpDataTables.pRareAffixDataTables.pRareAffixTxt, pRareSuffix, sizeof(D2RareAffixTxt) * nSuffixRecords);
	gpDataTables.pRareAffixDataTables.pRarePrefix = &gpDataTables.pRareAffixDataTables.pRareSuffix[nSuffixRecords];
	memcpy(&gpDataTables.pRareAffixDataTables.pRareSuffix[nSuffixRecords], pRarePrefix, sizeof(D2RareAffixTxt) * nPrefixRecords);

	DATATBLS_UnloadBin(pRarePrefix);
	DATATBLS_UnloadBin(pRareSuffix);

	for (int i = 0; i < gpDataTables.pRareAffixDataTables.nRareAffixTxtRecordCount; ++i)
	{
		gpDataTables.pRareAffixDataTables.pRareAffixTxt[i].wStringId = D2LANG_GetTblIndex(gpDataTables.pRareAffixDataTables.pRareAffixTxt[i].szName, &pUnicode);
	}
}

//D2Common.0x6FD58450
void __fastcall DATATBLS_UnloadRareSuffix_PrefixTxt()
{
	if (gpDataTables.pRareAffixDataTables.pRareAffixTxt)
	{
		FOG_FreeServerMemory(NULL, gpDataTables.pRareAffixDataTables.pRareAffixTxt, __FILE__, __LINE__, 0);
	}
	gpDataTables.pRareAffixDataTables.pRareAffixTxt = NULL;
}

//D2Common.0x6FD58480 (#10605)
D2RareAffixDataTbl* __fastcall DATATBLS_GetRareAffixDataTables()
{
	return &gpDataTables.pRareAffixDataTables;
}

//D2Common.0x6FD58490 (#10606)
D2RareAffixTxt* __stdcall DATATBLS_GetRareAffixTxtRecord(int nId)
{
	if (nId > gpDataTables.pRareAffixDataTables.nRareAffixTxtRecordCount || nId <= 0)
	{
		return NULL;
	}
	else
	{
		D2_ASSERT(gpDataTables.pRareAffixDataTables.pRareAffixTxt);
		return &gpDataTables.pRareAffixDataTables.pRareAffixTxt[nId - 1];
	}
}

//D2Common.0x6FD584E0
void __fastcall DATATBLS_LoadUniqueItemsTxt(void* pMemPool)
{
	wchar_t* pUnicode = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "index", TXTFIELD_ASCII, 31, 2, NULL },
		{ "version", TXTFIELD_WORD, 0, 36, NULL },
		{ "enabled", TXTFIELD_BIT, 0, 44, NULL },
		{ "ladder", TXTFIELD_BIT, 3, 44, NULL },
		{ "code", TXTFIELD_RAW, 0, 40, NULL },
		{ "rarity", TXTFIELD_WORD, 0, 48, NULL },
		{ "lvl", TXTFIELD_WORD, 0, 52, NULL },
		{ "lvl req", TXTFIELD_WORD, 0, 54, NULL },
		{ "nolimit", TXTFIELD_BIT, 1, 44, NULL },
		{ "carry1", TXTFIELD_BIT, 2, 44, NULL },
		{ "chrtransform", TXTFIELD_CODETOBYTE, 0, 56, &gpDataTables.pColorsLinker },
		{ "invtransform", TXTFIELD_CODETOBYTE, 0, 57, &gpDataTables.pColorsLinker },
		{ "flippyfile", TXTFIELD_ASCII, 31, 58, NULL },
		{ "invfile", TXTFIELD_ASCII, 31, 90, NULL },
		{ "cost mult", TXTFIELD_DWORD, 0, 124, NULL },
		{ "cost add", TXTFIELD_DWORD, 0, 128, NULL },
		{ "dropsound", TXTFIELD_NAMETOWORD, 0, 132, &gpDataTables.pSoundsLinker },
		{ "dropsfxframe", TXTFIELD_BYTE, 0, 136, NULL },
		{ "usesound", TXTFIELD_NAMETOWORD, 0, 134, &gpDataTables.pSoundsLinker },
		{ "prop1", TXTFIELD_NAMETODWORD, 0, 140, &gpDataTables.pPropertiesLinker },
		{ "par1", TXTFIELD_CALCTODWORD, 0, 144, DATATBLS_ItemParamLinker },
		{ "min1", TXTFIELD_DWORD, 0, 148, NULL },
		{ "max1", TXTFIELD_DWORD, 0, 152, NULL },
		{ "prop2", TXTFIELD_NAMETODWORD, 0, 156, &gpDataTables.pPropertiesLinker },
		{ "par2", TXTFIELD_CALCTODWORD, 0, 160, DATATBLS_ItemParamLinker },
		{ "min2", TXTFIELD_DWORD, 0, 164, NULL },
		{ "max2", TXTFIELD_DWORD, 0, 168, NULL },
		{ "prop3", TXTFIELD_NAMETODWORD, 0, 172, &gpDataTables.pPropertiesLinker },
		{ "par3", TXTFIELD_CALCTODWORD, 0, 176, DATATBLS_ItemParamLinker },
		{ "min3", TXTFIELD_DWORD, 0, 180, NULL },
		{ "max3", TXTFIELD_DWORD, 0, 184, NULL },
		{ "prop4", TXTFIELD_NAMETODWORD, 0, 188, &gpDataTables.pPropertiesLinker },
		{ "par4", TXTFIELD_CALCTODWORD, 0, 192, DATATBLS_ItemParamLinker },
		{ "min4", TXTFIELD_DWORD, 0, 196, NULL },
		{ "max4", TXTFIELD_DWORD, 0, 200, NULL },
		{ "prop5", TXTFIELD_NAMETODWORD, 0, 204, &gpDataTables.pPropertiesLinker },
		{ "par5", TXTFIELD_CALCTODWORD, 0, 208, DATATBLS_ItemParamLinker },
		{ "min5", TXTFIELD_DWORD, 0, 212, NULL },
		{ "max5", TXTFIELD_DWORD, 0, 216, NULL },
		{ "prop6", TXTFIELD_NAMETODWORD, 0, 220, &gpDataTables.pPropertiesLinker },
		{ "par6", TXTFIELD_CALCTODWORD, 0, 224, DATATBLS_ItemParamLinker },
		{ "min6", TXTFIELD_DWORD, 0, 228, NULL },
		{ "max6", TXTFIELD_DWORD, 0, 232, NULL },
		{ "prop7", TXTFIELD_NAMETODWORD, 0, 236, &gpDataTables.pPropertiesLinker },
		{ "par7", TXTFIELD_CALCTODWORD, 0, 240, DATATBLS_ItemParamLinker },
		{ "min7", TXTFIELD_DWORD, 0, 244, NULL },
		{ "max7", TXTFIELD_DWORD, 0, 248, NULL },
		{ "prop8", TXTFIELD_NAMETODWORD, 0, 252, &gpDataTables.pPropertiesLinker },
		{ "par8", TXTFIELD_CALCTODWORD, 0, 256, DATATBLS_ItemParamLinker },
		{ "min8", TXTFIELD_DWORD, 0, 260, NULL },
		{ "max8", TXTFIELD_DWORD, 0, 264, NULL },
		{ "prop9", TXTFIELD_NAMETODWORD, 0, 268, &gpDataTables.pPropertiesLinker },
		{ "par9", TXTFIELD_CALCTODWORD, 0, 272, DATATBLS_ItemParamLinker },
		{ "min9", TXTFIELD_DWORD, 0, 276, NULL },
		{ "max9", TXTFIELD_DWORD, 0, 280, NULL },
		{ "prop10", TXTFIELD_NAMETODWORD, 0, 284, &gpDataTables.pPropertiesLinker },
		{ "par10", TXTFIELD_CALCTODWORD, 0, 288, DATATBLS_ItemParamLinker },
		{ "min10", TXTFIELD_DWORD, 0, 292, NULL },
		{ "max10", TXTFIELD_DWORD, 0, 296, NULL },
		{ "prop11", TXTFIELD_NAMETODWORD, 0, 300, &gpDataTables.pPropertiesLinker },
		{ "par11", TXTFIELD_CALCTODWORD, 0, 304, DATATBLS_ItemParamLinker },
		{ "min11", TXTFIELD_DWORD, 0, 308, NULL },
		{ "max11", TXTFIELD_DWORD, 0, 312, NULL },
		{ "prop12", TXTFIELD_NAMETODWORD, 0, 316, &gpDataTables.pPropertiesLinker },
		{ "par12", TXTFIELD_CALCTODWORD, 0, 320, DATATBLS_ItemParamLinker },
		{ "min12", TXTFIELD_DWORD, 0, 324, NULL },
		{ "max12", TXTFIELD_DWORD, 0, 328, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pUniqueItemsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pUniqueItemsTxt = (D2UniqueItemsTxt*)DATATBLS_CompileTxt(pMemPool, "uniqueitems", pTbl, &gpDataTables.nUniqueItemsTxtRecordCount, sizeof(D2UniqueItemsTxt));

	if (gpDataTables.nUniqueItemsTxtRecordCount >= 32767)
	{
		FOG_10025("uniqueitems table exceeded maximum number of entries.", __FILE__, __LINE__);
	}

	for (int i = 0; i < gpDataTables.nUniqueItemsTxtRecordCount; ++i)
	{
		gpDataTables.pUniqueItemsTxt[i].wId = i;
		FOG_10216_AddRecordToLinkingTable(gpDataTables.pUniqueItemsLinker, gpDataTables.pUniqueItemsTxt[i].szName);

		gpDataTables.pUniqueItemsTxt[i].wTblIndex = D2LANG_GetTblIndex(gpDataTables.pUniqueItemsTxt[i].szName, &pUnicode);
		if (gpDataTables.pUniqueItemsTxt[i].wTblIndex == 0)
		{
			gpDataTables.pUniqueItemsTxt[i].wTblIndex = 5383;
		}
	}
}

//D2Common.0x6FD59110
void __fastcall DATATBLS_UnloadUniqueItemsTxt()
{
	FOG_FreeLinker(gpDataTables.pUniqueItemsLinker);
	DATATBLS_UnloadBin(gpDataTables.pUniqueItemsTxt);
	gpDataTables.nUniqueItemsTxtRecordCount = 0;
}

//D2Common.0x6FD59140
void __fastcall DATATBLS_LoadSets_SetItemsTxt(void* pMemPool)
{
	wchar_t* pUnicode = NULL;
	short nSetId = 0;

	D2BinFieldStrc pSetsTbl[] =
	{
		{ "index", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pSetsLinker },
		{ "name", TXTFIELD_KEYTOWORD, 0, TXTFIELD_DWORD, DATATBLS_GetStringIdFromReferenceString },
		{ "version", TXTFIELD_WORD, 0, 4, NULL },
		{ "pcode2a", TXTFIELD_NAMETODWORD, 0, 16, &gpDataTables.pPropertiesLinker },
		{ "pparam2a", TXTFIELD_CALCTODWORD, 0, 20, DATATBLS_ItemParamLinker },
		{ "pmin2a", TXTFIELD_DWORD, 0, 24, NULL },
		{ "pmax2a", TXTFIELD_DWORD, 0, 28, NULL },
		{ "pcode2b", TXTFIELD_NAMETODWORD, 0, 32, &gpDataTables.pPropertiesLinker },
		{ "pparam2b", TXTFIELD_CALCTODWORD, 0, 36, DATATBLS_ItemParamLinker },
		{ "pmin2b", TXTFIELD_DWORD, 0, 40, NULL },
		{ "pmax2b", TXTFIELD_DWORD, 0, 44, NULL },
		{ "pcode3a", TXTFIELD_NAMETODWORD, 0, 48, &gpDataTables.pPropertiesLinker },
		{ "pparam3a", TXTFIELD_CALCTODWORD, 0, 52, DATATBLS_ItemParamLinker },
		{ "pmin3a", TXTFIELD_DWORD, 0, 56, NULL },
		{ "pmax3a", TXTFIELD_DWORD, 0, 60, NULL },
		{ "pcode3b", TXTFIELD_NAMETODWORD, 0, 64, &gpDataTables.pPropertiesLinker },
		{ "pparam3b", TXTFIELD_CALCTODWORD, 0, 68, DATATBLS_ItemParamLinker },
		{ "pmin3b", TXTFIELD_DWORD, 0, 72, NULL },
		{ "pmax3b", TXTFIELD_DWORD, 0, 76, NULL },
		{ "pcode4a", TXTFIELD_NAMETODWORD, 0, 80, &gpDataTables.pPropertiesLinker },
		{ "pparam4a", TXTFIELD_CALCTODWORD, 0, 84, DATATBLS_ItemParamLinker },
		{ "pmin4a", TXTFIELD_DWORD, 0, 88, NULL },
		{ "pmax4a", TXTFIELD_DWORD, 0, 92, NULL },
		{ "pcode4b", TXTFIELD_NAMETODWORD, 0, 96, &gpDataTables.pPropertiesLinker },
		{ "pparam4b", TXTFIELD_CALCTODWORD, 0, 100, DATATBLS_ItemParamLinker },
		{ "pmin4b", TXTFIELD_DWORD, 0, 104, NULL },
		{ "pmax4b", TXTFIELD_DWORD, 0, 108, NULL },
		{ "pcode5a", TXTFIELD_NAMETODWORD, 0, 112, &gpDataTables.pPropertiesLinker },
		{ "pparam5a", TXTFIELD_CALCTODWORD, 0, 116, DATATBLS_ItemParamLinker },
		{ "pmin5a", TXTFIELD_DWORD, 0, 120, NULL },
		{ "pmax5a", TXTFIELD_DWORD, 0, 124, NULL },
		{ "pcode5b", TXTFIELD_NAMETODWORD, 0, 128, &gpDataTables.pPropertiesLinker },
		{ "pparam5b", TXTFIELD_CALCTODWORD, 0, 132, DATATBLS_ItemParamLinker },
		{ "pmin5b", TXTFIELD_DWORD, 0, 136, NULL },
		{ "pmax5b", TXTFIELD_DWORD, 0, 140, NULL },
		{ "fcode1", TXTFIELD_NAMETODWORD, 0, 144, &gpDataTables.pPropertiesLinker },
		{ "fparam1", TXTFIELD_CALCTODWORD, 0, 148, DATATBLS_ItemParamLinker },
		{ "fmin1", TXTFIELD_DWORD, 0, 152, NULL },
		{ "fmax1", TXTFIELD_DWORD, 0, 156, NULL },
		{ "fcode2", TXTFIELD_NAMETODWORD, 0, 160, &gpDataTables.pPropertiesLinker },
		{ "fparam2", TXTFIELD_CALCTODWORD, 0, 164, DATATBLS_ItemParamLinker },
		{ "fmin2", TXTFIELD_DWORD, 0, 168, NULL },
		{ "fmax2", TXTFIELD_DWORD, 0, 172, NULL },
		{ "fcode3", TXTFIELD_NAMETODWORD, 0, 176, &gpDataTables.pPropertiesLinker },
		{ "fparam3", TXTFIELD_CALCTODWORD, 0, 180, DATATBLS_ItemParamLinker },
		{ "fmin3", TXTFIELD_DWORD, 0, 184, NULL },
		{ "fmax3", TXTFIELD_DWORD, 0, 188, NULL },
		{ "fcode4", TXTFIELD_NAMETODWORD, 0, 192, &gpDataTables.pPropertiesLinker },
		{ "fparam4", TXTFIELD_CALCTODWORD, 0, 196, DATATBLS_ItemParamLinker },
		{ "fmin4", TXTFIELD_DWORD, 0, 200, NULL },
		{ "fmax4", TXTFIELD_DWORD, 0, 204, NULL },
		{ "fcode5", TXTFIELD_NAMETODWORD, 0, 208, &gpDataTables.pPropertiesLinker },
		{ "fparam5", TXTFIELD_CALCTODWORD, 0, 212, DATATBLS_ItemParamLinker },
		{ "fmin5", TXTFIELD_DWORD, 0, 216, NULL },
		{ "fmax5", TXTFIELD_DWORD, 0, 220, NULL },
		{ "fcode6", TXTFIELD_NAMETODWORD, 0, 224, &gpDataTables.pPropertiesLinker },
		{ "fparam6", TXTFIELD_CALCTODWORD, 0, 228, DATATBLS_ItemParamLinker },
		{ "fmin6", TXTFIELD_DWORD, 0, 232, NULL },
		{ "fmax6", TXTFIELD_DWORD, 0, 236, NULL },
		{ "fcode7", TXTFIELD_NAMETODWORD, 0, 240, &gpDataTables.pPropertiesLinker },
		{ "fparam7", TXTFIELD_CALCTODWORD, 0, 244, DATATBLS_ItemParamLinker },
		{ "fmin7", TXTFIELD_DWORD, 0, 248, NULL },
		{ "fmax7", TXTFIELD_DWORD, 0, 252, NULL },
		{ "fcode8", TXTFIELD_NAMETODWORD, 0, 256, &gpDataTables.pPropertiesLinker },
		{ "fparam8", TXTFIELD_CALCTODWORD, 0, 260, DATATBLS_ItemParamLinker },
		{ "fmin8", TXTFIELD_DWORD, 0, 264, NULL },
		{ "fmax8", TXTFIELD_DWORD, 0, 268, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	D2BinFieldStrc pSetItemsTbl[] =
	{
		{ "index", TXTFIELD_ASCII, 31, TXTFIELD_DWORD, NULL },
		{ "item", TXTFIELD_RAW, 0, 40, NULL },
		{ "set", TXTFIELD_NAMETOWORD, 0, 44, &gpDataTables.pSetsLinker },
		{ "lvl", TXTFIELD_WORD, 0, 48, NULL },
		{ "lvl req", TXTFIELD_WORD, 0, 50, NULL },
		{ "rarity", TXTFIELD_DWORD, 0, 52, NULL },
		{ "cost mult", TXTFIELD_DWORD, 0, 56, NULL },
		{ "cost add", TXTFIELD_DWORD, 0, 60, NULL },
		{ "chrtransform", TXTFIELD_CODETOBYTE, 0, 64, &gpDataTables.pColorsLinker },
		{ "invtransform", TXTFIELD_CODETOBYTE, 0, 65, &gpDataTables.pColorsLinker },
		{ "flippyfile", TXTFIELD_ASCII, 31, 66, NULL },
		{ "invfile", TXTFIELD_ASCII, 31, 98, NULL },
		{ "dropsound", TXTFIELD_NAMETOWORD, 0, 130, &gpDataTables.pSoundsLinker },
		{ "dropsfxframe", TXTFIELD_BYTE, 0, 134, NULL },
		{ "usesound", TXTFIELD_NAMETOWORD, 0, 132, &gpDataTables.pSoundsLinker },
		{ "add func", TXTFIELD_BYTE, 0, 135, NULL },
		{ "prop1", TXTFIELD_NAMETODWORD, 0, 136, &gpDataTables.pPropertiesLinker },
		{ "par1", TXTFIELD_CALCTODWORD, 0, 140, DATATBLS_ItemParamLinker },
		{ "min1", TXTFIELD_DWORD, 0, 144, NULL },
		{ "max1", TXTFIELD_DWORD, 0, 148, NULL },
		{ "prop2", TXTFIELD_NAMETODWORD, 0, 152, &gpDataTables.pPropertiesLinker },
		{ "par2", TXTFIELD_CALCTODWORD, 0, 156, DATATBLS_ItemParamLinker },
		{ "min2", TXTFIELD_DWORD, 0, 160, NULL },
		{ "max2", TXTFIELD_DWORD, 0, 164, NULL },
		{ "prop3", TXTFIELD_NAMETODWORD, 0, 168, &gpDataTables.pPropertiesLinker },
		{ "par3", TXTFIELD_CALCTODWORD, 0, 172, DATATBLS_ItemParamLinker },
		{ "min3", TXTFIELD_DWORD, 0, 176, NULL },
		{ "max3", TXTFIELD_DWORD, 0, 180, NULL },
		{ "prop4", TXTFIELD_NAMETODWORD, 0, 184, &gpDataTables.pPropertiesLinker },
		{ "par4", TXTFIELD_CALCTODWORD, 0, 188, DATATBLS_ItemParamLinker },
		{ "min4", TXTFIELD_DWORD, 0, 192, NULL },
		{ "max4", TXTFIELD_DWORD, 0, 196, NULL },
		{ "prop5", TXTFIELD_NAMETODWORD, 0, 200, &gpDataTables.pPropertiesLinker },
		{ "par5", TXTFIELD_CALCTODWORD, 0, 204, DATATBLS_ItemParamLinker },
		{ "min5", TXTFIELD_DWORD, 0, 208, NULL },
		{ "max5", TXTFIELD_DWORD, 0, 212, NULL },
		{ "prop6", TXTFIELD_NAMETODWORD, 0, 216, &gpDataTables.pPropertiesLinker },
		{ "par6", TXTFIELD_CALCTODWORD, 0, 220, DATATBLS_ItemParamLinker },
		{ "min6", TXTFIELD_DWORD, 0, 224, NULL },
		{ "max6", TXTFIELD_DWORD, 0, 228, NULL },
		{ "prop7", TXTFIELD_NAMETODWORD, 0, 232, &gpDataTables.pPropertiesLinker },
		{ "par7", TXTFIELD_CALCTODWORD, 0, 236, DATATBLS_ItemParamLinker },
		{ "min7", TXTFIELD_DWORD, 0, 240, NULL },
		{ "max7", TXTFIELD_DWORD, 0, 244, NULL },
		{ "prop8", TXTFIELD_NAMETODWORD, 0, 248, &gpDataTables.pPropertiesLinker },
		{ "par8", TXTFIELD_CALCTODWORD, 0, 252, DATATBLS_ItemParamLinker },
		{ "min8", TXTFIELD_DWORD, 0, 256, NULL },
		{ "max8", TXTFIELD_DWORD, 0, 260, NULL },
		{ "prop9", TXTFIELD_NAMETODWORD, 0, 264, &gpDataTables.pPropertiesLinker },
		{ "par9", TXTFIELD_CALCTODWORD, 0, 268, DATATBLS_ItemParamLinker },
		{ "min9", TXTFIELD_DWORD, 0, 272, NULL },
		{ "max9", TXTFIELD_DWORD, 0, 276, NULL },
		{ "aprop1a", TXTFIELD_NAMETODWORD, 0, 280, &gpDataTables.pPropertiesLinker },
		{ "apar1a", TXTFIELD_CALCTODWORD, 0, 284, DATATBLS_ItemParamLinker },
		{ "amin1a", TXTFIELD_DWORD, 0, 288, NULL },
		{ "amax1a", TXTFIELD_DWORD, 0, 292, NULL },
		{ "aprop1b", TXTFIELD_NAMETODWORD, 0, 296, &gpDataTables.pPropertiesLinker },
		{ "apar1b", TXTFIELD_CALCTODWORD, 0, 300, DATATBLS_ItemParamLinker },
		{ "amin1b", TXTFIELD_DWORD, 0, 304, NULL },
		{ "amax1b", TXTFIELD_DWORD, 0, 308, NULL },
		{ "aprop2a", TXTFIELD_NAMETODWORD, 0, 312, &gpDataTables.pPropertiesLinker },
		{ "apar2a", TXTFIELD_CALCTODWORD, 0, 316, DATATBLS_ItemParamLinker },
		{ "amin2a", TXTFIELD_DWORD, 0, 320, NULL },
		{ "amax2a", TXTFIELD_DWORD, 0, 324, NULL },
		{ "aprop2b", TXTFIELD_NAMETODWORD, 0, 328, &gpDataTables.pPropertiesLinker },
		{ "apar2b", TXTFIELD_CALCTODWORD, 0, 332, DATATBLS_ItemParamLinker },
		{ "amin2b", TXTFIELD_DWORD, 0, 336, NULL },
		{ "amax2b", TXTFIELD_DWORD, 0, 340, NULL },
		{ "aprop3a", TXTFIELD_NAMETODWORD, 0, 344, &gpDataTables.pPropertiesLinker },
		{ "apar3a", TXTFIELD_CALCTODWORD, 0, 348, DATATBLS_ItemParamLinker },
		{ "amin3a", TXTFIELD_DWORD, 0, 352, NULL },
		{ "amax3a", TXTFIELD_DWORD, 0, 356, NULL },
		{ "aprop3b", TXTFIELD_NAMETODWORD, 0, 360, &gpDataTables.pPropertiesLinker },
		{ "apar3b", TXTFIELD_CALCTODWORD, 0, 364, DATATBLS_ItemParamLinker },
		{ "amin3b", TXTFIELD_DWORD, 0, 368, NULL },
		{ "amax3b", TXTFIELD_DWORD, 0, 372, NULL },
		{ "aprop4a", TXTFIELD_NAMETODWORD, 0, 376, &gpDataTables.pPropertiesLinker },
		{ "apar4a", TXTFIELD_CALCTODWORD, 0, 380, DATATBLS_ItemParamLinker },
		{ "amin4a", TXTFIELD_DWORD, 0, 384, NULL },
		{ "amax4a", TXTFIELD_DWORD, 0, 388, NULL },
		{ "aprop4b", TXTFIELD_NAMETODWORD, 0, 392, &gpDataTables.pPropertiesLinker },
		{ "apar4b", TXTFIELD_CALCTODWORD, 0, 396, DATATBLS_ItemParamLinker },
		{ "amin4b", TXTFIELD_DWORD, 0, 400, NULL },
		{ "amax4b", TXTFIELD_DWORD, 0, 404, NULL },
		{ "aprop5a", TXTFIELD_NAMETODWORD, 0, 408, &gpDataTables.pPropertiesLinker },
		{ "apar5a", TXTFIELD_CALCTODWORD, 0, 412, DATATBLS_ItemParamLinker },
		{ "amin5a", TXTFIELD_DWORD, 0, 416, NULL },
		{ "amax5a", TXTFIELD_DWORD, 0, 420, NULL },
		{ "aprop5b", TXTFIELD_NAMETODWORD, 0, 424, &gpDataTables.pPropertiesLinker },
		{ "apar5b", TXTFIELD_CALCTODWORD, 0, 428, DATATBLS_ItemParamLinker },
		{ "amin5b", TXTFIELD_DWORD, 0, 432, NULL },
		{ "amax5b", TXTFIELD_DWORD, 0, 436, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pSetsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pSetsTxt = (D2SetsTxt*)DATATBLS_CompileTxt(pMemPool, "sets", pSetsTbl, &gpDataTables.nSetsTxtRecordCount, sizeof(D2SetsTxt));

	if (gpDataTables.nSetsTxtRecordCount >= 32767)
	{
		FOG_10025("sets table exceeded maximum number of entries.", __FILE__, __LINE__);
	}

	gpDataTables.pSetItemsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pSetItemsTxt = (D2SetItemsTxt*)DATATBLS_CompileTxt(pMemPool, "setitems", pSetItemsTbl, &gpDataTables.nSetItemsTxtRecordCount, sizeof(D2SetItemsTxt));

	if (gpDataTables.nSetItemsTxtRecordCount >= 32767)
	{
		FOG_10025("setitems table exceeded maximum number of entries.", __FILE__, __LINE__);
	}

	for (int i = 0; i < gpDataTables.nSetItemsTxtRecordCount; ++i)
	{
		gpDataTables.pSetItemsTxt[i].wSetItemId = i;
		FOG_10216_AddRecordToLinkingTable(gpDataTables.pSetItemsLinker, gpDataTables.pSetItemsTxt[i].szName);

		gpDataTables.pSetItemsTxt[i].wStringId = D2LANG_GetTblIndex(gpDataTables.pSetItemsTxt[i].szName, &pUnicode);
		if (!gpDataTables.pSetItemsTxt[i].wStringId)
		{
			gpDataTables.pSetItemsTxt[i].wStringId = 5383;
		}

		nSetId = gpDataTables.pSetItemsTxt[i].nSetId;
		if (nSetId >= 0 && nSetId < gpDataTables.nSetsTxtRecordCount)
		{
			if (gpDataTables.pSetsTxt[nSetId].nSetItems < 6)
			{
				gpDataTables.pSetItemsTxt[i].wVersion = gpDataTables.pSetsTxt[nSetId].wVersion;
				gpDataTables.pSetItemsTxt[i].nSetItems = gpDataTables.pSetsTxt[nSetId].nSetItems;
				gpDataTables.pSetsTxt[nSetId].pSetItem[gpDataTables.pSetsTxt[nSetId].nSetItems] = &gpDataTables.pSetItemsTxt[i];

				++gpDataTables.pSetsTxt[nSetId].nSetItems;//TODO: Increment SetItemsTxt-Counter, too? (Not done in the original code)
			}
			else
			{
				FOG_WriteToLogFile("Error: too many items in set %d", gpDataTables.pSetsTxt[nSetId].wSetId);
			}
		}
	}
}

//D2Common.0x6FD5AE00
void __fastcall DATATBLS_UnloadSets_SetItemsTxt()
{
	FOG_FreeLinker(gpDataTables.pSetsLinker);
	FOG_FreeLinker(gpDataTables.pSetItemsLinker);
	DATATBLS_UnloadBin(gpDataTables.pSetItemsTxt);
	DATATBLS_UnloadBin(gpDataTables.pSetsTxt);
	gpDataTables.nSetsTxtRecordCount = 0;
	gpDataTables.nSetItemsTxtRecordCount = 0;
}

//D2Common.0x6FD5AE40
void __fastcall DATATBLS_LoadQualityItemsTxt(void* pMemPool)
{
	wchar_t* pUnicode = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "nummods", TXTFIELD_BYTE, 0, 10, NULL },
		{ "mod1code", TXTFIELD_NAMETODWORD, 0, 12, &gpDataTables.pPropertiesLinker },
		{ "mod2code", TXTFIELD_NAMETODWORD, 0, 28, &gpDataTables.pPropertiesLinker },
		{ "mod1param", TXTFIELD_DWORD, 0, 16, NULL },
		{ "mod1min", TXTFIELD_DWORD, 0, 20, NULL },
		{ "mod1max", TXTFIELD_DWORD, 0, 24, NULL },
		{ "mod2param", TXTFIELD_DWORD, 0, 32, NULL },
		{ "mod2min", TXTFIELD_DWORD, 0, 36, NULL },
		{ "mod2max", TXTFIELD_DWORD, 0, 40, NULL },
		{ "effect1", TXTFIELD_ASCII, 31, 44, NULL },
		{ "effect2", TXTFIELD_ASCII, 31, 76, NULL },
		{ "armor", TXTFIELD_BYTE, 0, 0, NULL },
		{ "weapon", TXTFIELD_BYTE, 0, 1, NULL },
		{ "shield", TXTFIELD_BYTE, 0, 2, NULL },
		{ "scepter", TXTFIELD_BYTE, 0, 3, NULL },
		{ "wand", TXTFIELD_BYTE, 0, 4, NULL },
		{ "staff", TXTFIELD_BYTE, 0, 5, NULL },
		{ "bow", TXTFIELD_BYTE, 0, 6, NULL },
		{ "boots", TXTFIELD_BYTE, 0, 7, NULL },
		{ "gloves", TXTFIELD_BYTE, 0, 8, NULL },
		{ "belt", TXTFIELD_BYTE, 0, 9, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pQualityItemDataTables.pQualityItemsTxt = (D2QualityItemsTxt*)DATATBLS_CompileTxt(pMemPool, "qualityitems", pTbl, &gpDataTables.pQualityItemDataTables.nQualityItemsTxtRecordCount, sizeof(D2QualityItemsTxt));

	for (int i = 0; i < gpDataTables.pQualityItemDataTables.nQualityItemsTxtRecordCount; ++i)
	{
		if (gpDataTables.pQualityItemDataTables.pQualityItemsTxt[i].szEffect1[0])
		{
			gpDataTables.pQualityItemDataTables.pQualityItemsTxt[i].wEffect1TblId = D2LANG_GetTblIndex(gpDataTables.pQualityItemDataTables.pQualityItemsTxt[i].szEffect1, &pUnicode);
		}

		if (gpDataTables.pQualityItemDataTables.pQualityItemsTxt[i].szEffect2[0])
		{
			gpDataTables.pQualityItemDataTables.pQualityItemsTxt[i].wEffect2TblId = D2LANG_GetTblIndex(gpDataTables.pQualityItemDataTables.pQualityItemsTxt[i].szEffect2, &pUnicode);
		}
	}
}

//D2Common.0x6FD5B250
void __fastcall DATATBLS_UnloadQualityItemsTxt()
{
	DATATBLS_UnloadBin(gpDataTables.pQualityItemDataTables.pQualityItemsTxt);
}

//D2Common.0x6FD5B260 (#10611)
D2QualityItemDataTbl* __fastcall DATATBLS_GetQualityItemDataTables()
{
	return &gpDataTables.pQualityItemDataTables;
}

//D2Common.0x6FD5B270 (#10612)
D2QualityItemsTxt* __stdcall DATATBLS_GetQualityItemsTxtRecord(int nIndex)
{
	if (nIndex >= gpDataTables.pQualityItemDataTables.nQualityItemsTxtRecordCount || nIndex == -1)
	{
		return NULL;
	}
	else
	{
		D2_ASSERT(gpDataTables.pQualityItemDataTables.pQualityItemsTxt);
		D2_ASSERT(&gpDataTables.pQualityItemDataTables.pQualityItemsTxt[nIndex]);
		return &gpDataTables.pQualityItemDataTables.pQualityItemsTxt[nIndex];
	}
}

//D2Common.0x6FD5B2F0
void __fastcall DATATBLS_LoadGemsTxt(void* pMemPool)
{
	wchar_t* pUnicode = NULL;
	int nItemCode = 0;
	char szReference[4] = {};

	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_ASCII, 31, 0, NULL },
		{ "letter", TXTFIELD_ASCII, 5, 32, NULL },
		{ "code", TXTFIELD_UNKNOWN3, 0, 40, &gpDataTables.pItemsLinker },
		{ "transform", TXTFIELD_BYTE, 0, 47, NULL },
		{ "nummods", TXTFIELD_BYTE, 0, 46, NULL },
		{ "weaponmod1code", TXTFIELD_NAMETODWORD, 0, 48, &gpDataTables.pPropertiesLinker },
		{ "weaponmod1param", TXTFIELD_CALCTODWORD, 0, 52, DATATBLS_ItemParamLinker },
		{ "weaponmod1min", TXTFIELD_DWORD, 0, 56, NULL },
		{ "weaponmod1max", TXTFIELD_DWORD, 0, 60, NULL },
		{ "weaponmod2code", TXTFIELD_NAMETODWORD, 0, 64, &gpDataTables.pPropertiesLinker },
		{ "weaponmod2param", TXTFIELD_CALCTODWORD, 0, 68, DATATBLS_ItemParamLinker },
		{ "weaponmod2min", TXTFIELD_DWORD, 0, 72, NULL },
		{ "weaponmod2max", TXTFIELD_DWORD, 0, 76, NULL },
		{ "weaponmod3code", TXTFIELD_NAMETODWORD, 0, 80, &gpDataTables.pPropertiesLinker },
		{ "weaponmod3param", TXTFIELD_CALCTODWORD, 0, 84, DATATBLS_ItemParamLinker },
		{ "weaponmod3min", TXTFIELD_DWORD, 0, 88, NULL },
		{ "weaponmod3max", TXTFIELD_DWORD, 0, 92, NULL },
		{ "helmmod1code", TXTFIELD_NAMETODWORD, 0, 96, &gpDataTables.pPropertiesLinker },
		{ "helmmod1param", TXTFIELD_CALCTODWORD, 0, 100, DATATBLS_ItemParamLinker },
		{ "helmmod1min", TXTFIELD_DWORD, 0, 104, NULL },
		{ "helmmod1max", TXTFIELD_DWORD, 0, 108, NULL },
		{ "helmmod2code", TXTFIELD_NAMETODWORD, 0, 112, &gpDataTables.pPropertiesLinker },
		{ "helmmod2param", TXTFIELD_CALCTODWORD, 0, 116, DATATBLS_ItemParamLinker },
		{ "helmmod2min", TXTFIELD_DWORD, 0, 120, NULL },
		{ "helmmod2max", TXTFIELD_DWORD, 0, 124, NULL },
		{ "helmmod3code", TXTFIELD_NAMETODWORD, 0, 128, &gpDataTables.pPropertiesLinker },
		{ "helmmod3param", TXTFIELD_CALCTODWORD, 0, 132, DATATBLS_ItemParamLinker },
		{ "helmmod3min", TXTFIELD_DWORD, 0, 136, NULL },
		{ "helmmod3max", TXTFIELD_DWORD, 0, 140, NULL },
		{ "shieldmod1code", TXTFIELD_NAMETODWORD, 0, 144, &gpDataTables.pPropertiesLinker },
		{ "shieldmod1param", TXTFIELD_CALCTODWORD, 0, 148, DATATBLS_ItemParamLinker },
		{ "shieldmod1min", TXTFIELD_DWORD, 0, 152, NULL },
		{ "shieldmod1max", TXTFIELD_DWORD, 0, 156, NULL },
		{ "shieldmod2code", TXTFIELD_NAMETODWORD, 0, 160, &gpDataTables.pPropertiesLinker },
		{ "shieldmod2param", TXTFIELD_CALCTODWORD, 0, 164, DATATBLS_ItemParamLinker },
		{ "shieldmod2min", TXTFIELD_DWORD, 0, 168, NULL },
		{ "shieldmod2max", TXTFIELD_DWORD, 0, 172, NULL },
		{ "shieldmod3code", TXTFIELD_NAMETODWORD, 0, 176, &gpDataTables.pPropertiesLinker },
		{ "shieldmod3param", TXTFIELD_CALCTODWORD, 0, 180, DATATBLS_ItemParamLinker },
		{ "shieldmod3min", TXTFIELD_DWORD, 0, 184, NULL },
		{ "shieldmod3max", TXTFIELD_DWORD, 0, 188, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pGemDataTables.pGemsTxt = (D2GemsTxt*)DATATBLS_CompileTxt(pMemPool, "gems", pTbl, &gpDataTables.pGemDataTables.nGemsTxtRecordCount, sizeof(D2GemsTxt));


	for (int i = 0; i < gpDataTables.pGemDataTables.nGemsTxtRecordCount; ++i)
	{
		szReference[0] = gpDataTables.pGemDataTables.pGemsTxt[i].szItemCode[0] & ((gpDataTables.pGemDataTables.pGemsTxt[i].szItemCode[0] == ' ') - 1);
		szReference[1] = gpDataTables.pGemDataTables.pGemsTxt[i].szItemCode[1] & ((gpDataTables.pGemDataTables.pGemsTxt[i].szItemCode[1] == ' ') - 1);
		szReference[2] = gpDataTables.pGemDataTables.pGemsTxt[i].szItemCode[2] & ((gpDataTables.pGemDataTables.pGemsTxt[i].szItemCode[2] == ' ') - 1);
		szReference[3] = 0;

		gpDataTables.pGemDataTables.pGemsTxt[i].wStringId = D2LANG_GetTblIndex(szReference, &pUnicode);
	}

	for (int i = 0; i < gpDataTables.pGemDataTables.nGemsTxtRecordCount; ++i)
	{
		gpDataTables.pItemDataTables.pItemsTxt[i].dwGemOffset = -1;
		nItemCode = gpDataTables.pGemDataTables.pGemsTxt[i].dwItemCode;
		if (nItemCode >= 0)
		{
			gpDataTables.pItemDataTables.pItemsTxt[nItemCode].dwGemOffset = i;
		}
	}
}

//D2Common.0x6FD5BAE0
void __fastcall DATATBLS_UnloadGemsTxt()
{
	DATATBLS_UnloadBin(gpDataTables.pGemDataTables.pGemsTxt);
}

//D2Common.0x6FD5BAF0 (#10615)
D2GemDataTbl* __fastcall DATATBLS_GetGemDataTables()
{
	return &gpDataTables.pGemDataTables;
}

//D2Common.0x6FD5BB00 (#10616)
D2GemsTxt* __stdcall DATATBLS_GetGemsTxtRecord(int nGemId)
{
	if (nGemId >= gpDataTables.pGemDataTables.nGemsTxtRecordCount || nGemId == -1)
	{
		return NULL;
	}
	else
	{
		D2_ASSERT(gpDataTables.pGemDataTables.pGemsTxt);
		D2_ASSERT(&gpDataTables.pGemDataTables.pGemsTxt[nGemId]);
		return &gpDataTables.pGemDataTables.pGemsTxt[nGemId];
	}
}

//D2Common.0x6FD5BB70
void __fastcall DATATBLS_LoadBooksTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_KEYTOWORD, 0, 0, DATATBLS_GetStringIdFromReferenceString },
		{ "scrollspellcode", TXTFIELD_RAW, 0, 24, NULL },
		{ "bookspellcode", TXTFIELD_RAW, 0, 28, NULL },
		{ "pSpell", TXTFIELD_DWORD, 0, 4, NULL },
		{ "scrollskill", TXTFIELD_NAMETODWORD, 0, 8, &gpDataTables.pSkillsLinker },
		{ "bookskill", TXTFIELD_NAMETODWORD, 0, 12, &gpDataTables.pSkillsLinker },
		{ "spellicon", TXTFIELD_BYTE, 0, 2, NULL },
		{ "basecost", TXTFIELD_DWORD, 0, 16, NULL },
		{ "costpercharge", TXTFIELD_DWORD, 0, 20, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pBookDataTables.pBooksTxt = (D2BooksTxt*)DATATBLS_CompileTxt(pMemPool, "books", pTbl, &gpDataTables.pBookDataTables.nBooksTxtRecordCount, sizeof(D2BooksTxt));
}

//D2Common.0x6FD5BD10
void __fastcall DATATBLS_UnloadBooksTxt()
{
	DATATBLS_UnloadBin(gpDataTables.pBookDataTables.pBooksTxt);
}

//D2Common.0x6FD5BD20 (#10617)
D2BookDataTbl* __fastcall DATATBLS_GetBookDataTables()
{
	return &gpDataTables.pBookDataTables;
}

//D2Common.0x6FD5BD30 (#10618)
D2BooksTxt* __stdcall DATATBLS_GetBooksTxtRecord(int nBookId)
{
	if (nBookId >= gpDataTables.pBookDataTables.nBooksTxtRecordCount || nBookId == -1)
	{
		return NULL;
	}
	else
	{
		D2_ASSERT(gpDataTables.pBookDataTables.pBooksTxt);
		D2_ASSERT(&gpDataTables.pBookDataTables.pBooksTxt[nBookId]);

		return &gpDataTables.pBookDataTables.pBooksTxt[nBookId];
	}
}

//D2Common.0x6FD5BDA0
void __fastcall DATATBLS_LoadLowQualityItemsTxt(void* pMemPool)
{
	wchar_t* pUnicode = NULL;
	D2BinFieldStrc pTbl[] =
	{
		{ "Name", TXTFIELD_ASCII, 31, 0, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pLowQualityItemDataTables.pLowQualityItemsTxt = (D2LowQualityItemsTxt *)DATATBLS_CompileTxt(pMemPool, "lowqualityitems", pTbl, &gpDataTables.pLowQualityItemDataTables.nLowQualityItemsTxtRecordCount, sizeof(D2LowQualityItemsTxt));

	for (int i = 0; i < gpDataTables.pLowQualityItemDataTables.nLowQualityItemsTxtRecordCount; ++i)
	{
		gpDataTables.pLowQualityItemDataTables.pLowQualityItemsTxt[i].wTblId = D2LANG_GetTblIndex(gpDataTables.pLowQualityItemDataTables.pLowQualityItemsTxt[i].szName, &pUnicode);
	}
}

//D2Common.0x6FD5BE40
void __fastcall DATATBLS_UnloadLowQualityItemsTxt()
{
	DATATBLS_UnloadBin(gpDataTables.pLowQualityItemDataTables.pLowQualityItemsTxt);
}

//D2Common.0x6FD5BE50 (#10613)
D2LowQualityItemDataTbl* __fastcall DATATBLS_GetLowQualityItemDataTables()
{
	return &gpDataTables.pLowQualityItemDataTables;
}

//D2Common.0x6FD5BE60 (#10614)
D2LowQualityItemsTxt* __stdcall DATATBLS_GetLowQualityItemsTxtRecord(int nId)
{
	if (nId >= gpDataTables.pLowQualityItemDataTables.nLowQualityItemsTxtRecordCount || nId == -1)
	{
		return NULL;
	}
	else
	{
		D2_ASSERT(gpDataTables.pLowQualityItemDataTables.pLowQualityItemsTxt);
		D2_ASSERT(&gpDataTables.pLowQualityItemDataTables.pLowQualityItemsTxt[nId]);

		return &gpDataTables.pLowQualityItemDataTables.pLowQualityItemsTxt[nId];
	}
}

//D2Common.0x6FD5BED0
void __fastcall DATATBLS_LoadItemRatioTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Version", TXTFIELD_WORD, 0, 64, NULL },
		{ "Uber", TXTFIELD_BYTE, 0, 66, NULL },
		{ "Class Specific", TXTFIELD_BYTE, 0, 67, NULL },
		{ "Unique", TXTFIELD_DWORD, 0, 0, NULL },
		{ "UniqueDivisor", TXTFIELD_DWORD, 0, 4, NULL },
		{ "UniqueMin", TXTFIELD_DWORD, 0, 8, NULL },
		{ "Rare", TXTFIELD_DWORD, 0, 12, NULL },
		{ "RareDivisor", TXTFIELD_DWORD, 0, 16, NULL },
		{ "RareMin", TXTFIELD_DWORD, 0, 20, NULL },
		{ "Set", TXTFIELD_DWORD, 0, 24, NULL },
		{ "SetDivisor", TXTFIELD_DWORD, 0, 28, NULL },
		{ "SetMin", TXTFIELD_DWORD, 0, 32, NULL },
		{ "Magic", TXTFIELD_DWORD, 0, 36, NULL },
		{ "MagicDivisor", TXTFIELD_DWORD, 0, 40, NULL },
		{ "MagicMin", TXTFIELD_DWORD, 0, 44, NULL },
		{ "HiQuality", TXTFIELD_DWORD, 0, 48, NULL },
		{ "HiQualityDivisor", TXTFIELD_DWORD, 0, 52, NULL },
		{ "Normal", TXTFIELD_DWORD, 0, 56, NULL },
		{ "NormalDivisor", TXTFIELD_DWORD, 0, 60, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pItemRatioDataTables.pItemRatioTxt = (D2ItemRatioTxt*)DATATBLS_CompileTxt(pMemPool, "itemratio", pTbl, &gpDataTables.pItemRatioDataTables.nItemRatioTxtRecordCount, sizeof(D2ItemRatioTxt));
}

//D2Common.0x6FD5C200
void __fastcall DATATBLS_UnloadItemRatioTxt()
{
	DATATBLS_UnloadBin(gpDataTables.pItemRatioDataTables.pItemRatioTxt);
}

//D2Common.0x6FD5C210 (#10622)
D2ItemRatioDataTbl* __fastcall DATATBLS_GetItemRatioDataTables()
{
	return &gpDataTables.pItemRatioDataTables;
}

//D2Common.0x6FD5C220 (#10623)
D2ItemRatioTxt* __stdcall DATATBLS_GetItemRatioTxtRecord(int nItemId, BYTE nDifficulty, WORD wVersion)
{
	int nClass = 0;
	int nQuest = 0;
	int nId = -1;
	int nLastVersion = -1;

	nClass = ITEMS_IsClassValidByItemId(nItemId);
	nQuest = ITEMS_GetQuestFromItemId(nItemId);

#define ITEMS_PRE_EXPANSION_VERSION 0
#define ITEMS_EXPANSION_VERSION_BASE 100
	D2_ASSERT(wVersion == ITEMS_PRE_EXPANSION_VERSION || wVersion == ITEMS_EXPANSION_VERSION_BASE);

	for (int i = 0; i < gpDataTables.pItemRatioDataTables.nItemRatioTxtRecordCount; ++i)
	{
		if (nClass == gpDataTables.pItemRatioDataTables.pItemRatioTxt[i].nClassSpecific
			&& nQuest == gpDataTables.pItemRatioDataTables.pItemRatioTxt[i].nUber
			&& gpDataTables.pItemRatioDataTables.pItemRatioTxt[i].wVersion <= wVersion
			&& gpDataTables.pItemRatioDataTables.pItemRatioTxt[i].wVersion >= nLastVersion)
		{
			nLastVersion = gpDataTables.pItemRatioDataTables.pItemRatioTxt[i].wVersion;
			nId = i;
		}
	}

	if (nId >= 0)
	{
		return &gpDataTables.pItemRatioDataTables.pItemRatioTxt[nId];
	}

	return NULL;
}

//D2Common.0x6FD5C2F0
int __cdecl DATATBLS_CompareItemStatCostDescs(const void* pRecord1, const void* pRecord2)
{
	D2ItemStatCostDescStrc* pElem1 = (D2ItemStatCostDescStrc*)pRecord1;
	D2ItemStatCostDescStrc* pElem2 = (D2ItemStatCostDescStrc*)pRecord2;

	if (pElem1->nDescPriority > pElem2->nDescPriority)
	{
		return 1;
	}
	else if (pElem1->nDescPriority == pElem2->nDescPriority)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

//D2Common.0x6FD5C320
void __fastcall DATATBLS_LoadItemStatCostTxt(void* pMemPool)
{
	int nStatsWithDescFunc = 0;
	int nNextFreeId = 0;
	int nOpCounter = 0;
	WORD nOpBase = 0;
	WORD* pOpStat = NULL;
	BYTE* pOp = NULL;
	D2ItemStatCostDescStrc pStatsWithDescFunc[511] = {};

	D2BinFieldStrc pTbl[] =
	{
		{ "stat", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pItemStatCostLinker },
		{ "send bits", TXTFIELD_BYTE, 0, 8, NULL },
		{ "send param bits", TXTFIELD_BYTE, 0, 9, NULL },
		{ "divide", TXTFIELD_DWORD, 0, 12, NULL },
		{ "multiply", TXTFIELD_DWORD, 0, 16, NULL },
		{ "add", TXTFIELD_DWORD, 0, 20, NULL },
		{ "valshift", TXTFIELD_BYTE, 0, 24, NULL },
		{ "minaccr", TXTFIELD_DWORD, 0, 44, NULL },
		{ "save bits", TXTFIELD_BYTE, 0, 25, NULL },
		{ "save add", TXTFIELD_DWORD, 0, 28, NULL },
		{ "save param bits", TXTFIELD_DWORD, 0, 36, NULL },
		{ "1.09-save bits", TXTFIELD_BYTE, 0, 26, NULL },
		{ "1.09-save add", TXTFIELD_DWORD, 0, 32, NULL },
		{ "encode", TXTFIELD_BYTE, 0, 48, NULL },
		{ "send other", TXTFIELD_BIT, 0, 4, NULL },
		{ "signed", TXTFIELD_BIT, 1, 4, NULL },
		{ "updateanimrate", TXTFIELD_BIT, 9, 4, NULL },
		{ "fmin", TXTFIELD_BIT, 10, 4, NULL },
		{ "damagerelated", TXTFIELD_BIT, 2, 4, NULL },
		{ "itemspecific", TXTFIELD_BIT, 3, 4, NULL },
		{ "direct", TXTFIELD_BIT, 4, 4, NULL },
		{ "fcallback", TXTFIELD_BIT, 11, 4, NULL },
		{ "saved", TXTFIELD_BIT, 12, 4, NULL },
		{ "csvsigned", TXTFIELD_BIT, 13, 4, NULL },
		{ "csvbits", TXTFIELD_BYTE, 0, 10, NULL },
		{ "csvparam", TXTFIELD_BYTE, 0, 11, NULL },
		{ "maxstat", TXTFIELD_NAMETOWORD, 0, 50, &gpDataTables.pItemStatCostLinker },
		{ "itemevent1", TXTFIELD_NAMETOWORD, 0, 72, &gpDataTables.pEventsLinker },
		{ "itemeventfunc1", TXTFIELD_WORD, 0, 76, NULL },
		{ "itemevent2", TXTFIELD_NAMETOWORD, 0, 74, &gpDataTables.pEventsLinker },
		{ "itemeventfunc2", TXTFIELD_WORD, 0, 78, NULL },
		{ "descpriority", TXTFIELD_WORD, 0, 52, NULL },
		{ "descfunc", TXTFIELD_BYTE, 0, 54, NULL },
		{ "descval", TXTFIELD_BYTE, 0, 55, NULL },
		{ "descstrpos", TXTFIELD_KEYTOWORD, 0, 56, DATATBLS_GetStringIdFromReferenceString },
		{ "descstrneg", TXTFIELD_KEYTOWORD, 0, 58, DATATBLS_GetStringIdFromReferenceString },
		{ "descstr2", TXTFIELD_KEYTOWORD, 0, 60, DATATBLS_GetStringIdFromReferenceString },
		{ "dgrp", TXTFIELD_WORD, 0, 62, NULL },
		{ "dgrpfunc", TXTFIELD_BYTE, 0, 64, NULL },
		{ "dgrpval", TXTFIELD_BYTE, 0, 65, NULL },
		{ "dgrpstrpos", TXTFIELD_KEYTOWORD, 0, 66, DATATBLS_GetStringIdFromReferenceString },
		{ "dgrpstrneg", TXTFIELD_KEYTOWORD, 0, 68, DATATBLS_GetStringIdFromReferenceString },
		{ "dgrpstr2", TXTFIELD_KEYTOWORD, 0, 70, DATATBLS_GetStringIdFromReferenceString },
		{ "keepzero", TXTFIELD_BYTE, 0, 80, NULL },
		{ "op", TXTFIELD_BYTE, 0, 84, NULL },
		{ "op base", TXTFIELD_NAMETOWORD, 0, 86, &gpDataTables.pItemStatCostLinker },
		{ "op param", TXTFIELD_BYTE, 0, 85, NULL },
		{ "op stat1", TXTFIELD_NAMETOWORD, 0, 88, &gpDataTables.pItemStatCostLinker },
		{ "op stat2", TXTFIELD_NAMETOWORD, 0, 90, &gpDataTables.pItemStatCostLinker },
		{ "op stat3", TXTFIELD_NAMETOWORD, 0, 92, &gpDataTables.pItemStatCostLinker },
		{ "stuff", TXTFIELD_DWORD, 0, 320, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pItemStatCostLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pItemStatCostTxt = (D2ItemStatCostTxt*)DATATBLS_CompileTxt(pMemPool, "itemstatcost", pTbl, &gpDataTables.nItemStatCostTxtRecordCount, sizeof(D2ItemStatCostTxt));

#define MAX_STATS 511
	D2_ASSERT(gpDataTables.nItemStatCostTxtRecordCount <= MAX_STATS);

	gpDataTables.nStuff = gpDataTables.pItemStatCostTxt->dwStuff;
	if (gpDataTables.nStuff <= 0 || gpDataTables.nStuff > 8)
	{
		gpDataTables.nStuff = 6;
	}
	gpDataTables.nShiftedStuff = (1 << gpDataTables.nStuff) - 1;

	for (int i = 0; i < gpDataTables.nItemStatCostTxtRecordCount; ++i)
	{
		memset(gpDataTables.pItemStatCostTxt[i].unk0x5E, -1, sizeof(gpDataTables.pItemStatCostTxt[i].unk0x5E));
	}

	for (int nStatId = 0; nStatId < gpDataTables.nItemStatCostTxtRecordCount; ++nStatId)
	{
		pOp = &gpDataTables.pItemStatCostTxt[nStatId].nOp;

		if (*pOp > 0 && *pOp < 14)
		{
			nOpBase = gpDataTables.pItemStatCostTxt[nStatId].wOpBase;

			if (nOpBase < gpDataTables.nItemStatCostTxtRecordCount)
			{
				gpDataTables.pItemStatCostTxt[nOpBase].unk0x51[0] = 1;

				nNextFreeId = 0;
				while (nNextFreeId < ARRAY_SIZE(gpDataTables.pItemStatCostTxt[nOpBase].unk0x5E))
				{
					if (gpDataTables.pItemStatCostTxt[nOpBase].unk0x5E[nNextFreeId] >= gpDataTables.nItemStatCostTxtRecordCount)
					{
						gpDataTables.pItemStatCostTxt[nOpBase].unk0x5E[nNextFreeId] = nStatId;
						break;
					}

					++nNextFreeId;
				}

				if (nNextFreeId >= ARRAY_SIZE(gpDataTables.pItemStatCostTxt[nOpBase].unk0x5E) && gpDataTables.bCompileTxt)
				{
					FOG_WriteToLogFile("Error: greater than %d ops applied to target %s\n", 3, FOG_10255(gpDataTables.pItemStatCostLinker, nOpBase, 0));
				}

				if (*pOp == 4 || *pOp == 5)
				{
					gpDataTables.pItemStatCostTxt[nStatId].unk0x51[2] = 1;
				}
			}

			pOpStat = gpDataTables.pItemStatCostTxt[nStatId].wOpStat;
			for (int nOpCounter = 0; nOpCounter < ARRAY_SIZE(gpDataTables.pItemStatCostTxt[nStatId].wOpStat); ++nOpCounter)
			{
				if (pOpStat[nOpCounter] >= gpDataTables.nItemStatCostTxtRecordCount)
				{
					break;
				}

				gpDataTables.pItemStatCostTxt[nStatId].unk0x51[0] = 1;

				nNextFreeId = 0;
				while (nNextFreeId < 16)
				{
					if (!gpDataTables.pItemStatCostTxt[pOpStat[nOpCounter]].pOpStatData[nNextFreeId].nOp)
					{
						gpDataTables.pItemStatCostTxt[pOpStat[nOpCounter]].pOpStatData[nNextFreeId].nStat = nStatId;
						gpDataTables.pItemStatCostTxt[pOpStat[nOpCounter]].pOpStatData[nNextFreeId].nOp = *pOp;
						gpDataTables.pItemStatCostTxt[pOpStat[nOpCounter]].pOpStatData[nNextFreeId].nOpBase = nOpBase;
						gpDataTables.pItemStatCostTxt[pOpStat[nOpCounter]].pOpStatData[nNextFreeId].nOpParam = gpDataTables.pItemStatCostTxt[nStatId].nOpParam;

						gpDataTables.pItemStatCostTxt[pOpStat[nOpCounter]].unk0x51[1] = 1;

						if (nStatId == STAT_MAXHP || pOpStat[nOpCounter] == STAT_MAXHP)
						{
							gpDataTables.pItemStatCostTxt[nStatId].dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_HP];
							gpDataTables.pItemStatCostTxt[nStatId].dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_HP_MANA_STAMINA];
						}
						else if (nStatId == STAT_MAXMANA || pOpStat[nOpCounter] == STAT_MAXMANA)
						{
							gpDataTables.pItemStatCostTxt[nStatId].dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_MANA];
							gpDataTables.pItemStatCostTxt[nStatId].dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_HP_MANA_STAMINA];
						}
						else if (nStatId == STAT_MAXSTAMINA || pOpStat[nOpCounter] == STAT_MAXSTAMINA)
						{
							gpDataTables.pItemStatCostTxt[nStatId].dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_STAMINA];
							gpDataTables.pItemStatCostTxt[nStatId].dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_HP_MANA_STAMINA];
						}
						break;
					}

					++nNextFreeId;
				}

				if (nNextFreeId >= 16 && gpDataTables.bCompileTxt)
				{
					FOG_WriteToLogFile("Error: greater than %d ops applied to target %s\n", 16, FOG_10255(gpDataTables.pItemStatCostLinker, gpDataTables.pItemStatCostTxt[pOpStat[nOpCounter]].wStatId, 0));
				}
			}
		}
		else
		{
			*pOp = 0;
		}
	}

	nStatsWithDescFunc = 0;
	for (int i = 0; i < gpDataTables.nItemStatCostTxtRecordCount; ++i)
	{
		if (gpDataTables.pItemStatCostTxt[i].nDescFunc)
		{
			pStatsWithDescFunc[nStatsWithDescFunc].nRecordId = i;
			pStatsWithDescFunc[nStatsWithDescFunc].nDescPriority = gpDataTables.pItemStatCostTxt[i].nDescPriority;
			++nStatsWithDescFunc;
		}
	}
	gpDataTables.nStatsWithDescFunc = nStatsWithDescFunc;

	qsort(pStatsWithDescFunc, nStatsWithDescFunc, sizeof(D2ItemStatCostDescStrc), DATATBLS_CompareItemStatCostDescs);

	gpDataTables.pStatsWithDescFunc = (WORD*)FOG_AllocServerMemory(NULL, sizeof(WORD) * gpDataTables.nStatsWithDescFunc, __FILE__, __LINE__, 0);
	for (int i = 0; i < gpDataTables.nStatsWithDescFunc; ++i)
	{
		gpDataTables.pStatsWithDescFunc[i] = pStatsWithDescFunc[i].nRecordId;
	}
}

//D2Common.0x6FD5D070
void __fastcall DATATBLS_UnloadItemStatCostTxt()
{
	if (gpDataTables.pItemStatCostTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pItemStatCostTxt);
	}
	gpDataTables.pItemStatCostTxt = NULL;

	if (gpDataTables.pItemStatCostLinker)
	{
		FOG_FreeLinker(gpDataTables.pItemStatCostLinker);
	}
	gpDataTables.pItemStatCostLinker = NULL;
	gpDataTables.nItemStatCostTxtRecordCount = 0;

	if (gpDataTables.pStatsWithDescFunc)
	{
		FOG_FreeServerMemory(NULL, gpDataTables.pStatsWithDescFunc, __FILE__, __LINE__, 0);
	}
	gpDataTables.nStatsWithDescFunc = 0;
}

//D2Common.0x6FD5D0D0
void __fastcall DATATBLS_LoadPropertiesTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "code", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pPropertiesLinker },
		{ "set1", TXTFIELD_BYTE, 0, 2, NULL },
		{ "val1", TXTFIELD_WORD, 0, 10, NULL },
		{ "func1", TXTFIELD_BYTE, 0, 24, NULL },
		{ "stat1", TXTFIELD_NAMETOWORD, 0, 32, &gpDataTables.pItemStatCostLinker },
		{ "set2", TXTFIELD_BYTE, 0, 3, NULL },
		{ "val2", TXTFIELD_WORD, 0, 12, NULL },
		{ "func2", TXTFIELD_BYTE, 0, 25, NULL },
		{ "stat2", TXTFIELD_NAMETOWORD, 0, 34, &gpDataTables.pItemStatCostLinker },
		{ "set3", TXTFIELD_BYTE, 0, 4, NULL },
		{ "val3", TXTFIELD_WORD, 0, 14, NULL },
		{ "func3", TXTFIELD_BYTE, 0, 26, NULL },
		{ "stat3", TXTFIELD_NAMETOWORD, 0, 36, &gpDataTables.pItemStatCostLinker },
		{ "set4", TXTFIELD_BYTE, 0, 5, NULL },
		{ "val4", TXTFIELD_WORD, 0, 16, NULL },
		{ "func4", TXTFIELD_BYTE, 0, 27, NULL },
		{ "stat4", TXTFIELD_NAMETOWORD, 0, 38, &gpDataTables.pItemStatCostLinker },
		{ "set5", TXTFIELD_BYTE, 0, 6, NULL },
		{ "val5", TXTFIELD_WORD, 0, 18, NULL },
		{ "func5", TXTFIELD_BYTE, 0, 28, NULL },
		{ "stat5", TXTFIELD_NAMETOWORD, 0, 40, &gpDataTables.pItemStatCostLinker },
		{ "set6", TXTFIELD_BYTE, 0, 7, NULL },
		{ "val6", TXTFIELD_WORD, 0, TXTFIELD_NAMETOWORD, NULL },
		{ "func6", TXTFIELD_BYTE, 0, 29, NULL },
		{ "stat6", TXTFIELD_NAMETOWORD, 0, 42, &gpDataTables.pItemStatCostLinker },
		{ "set7", TXTFIELD_BYTE, 0, 8, NULL },
		{ "val7", TXTFIELD_WORD, 0, 22, NULL },
		{ "func7", TXTFIELD_BYTE, 0, 30, NULL },
		{ "stat7", TXTFIELD_NAMETOWORD, 0, 44, &gpDataTables.pItemStatCostLinker },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pPropertiesLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pPropertiesTxt = (D2PropertiesTxt*)DATATBLS_CompileTxt(pMemPool, "properties", pTbl, &gpDataTables.nPropertiesTxtRecordCount, sizeof(D2PropertiesTxt));
}

//D2Common.0x6FD5D5E0
void __fastcall DATATBLS_UnloadPropertiesTxt()
{
	DATATBLS_UnloadBin(gpDataTables.pPropertiesTxt);
	FOG_FreeLinker(gpDataTables.pPropertiesLinker);
}

//D2Common.0x6FD5D600
void __fastcall DATATBLS_LoadGambleTxt(void* pMemPool)
{
	int nCounter = 0;
	int nItemId = 0;
	D2GambleTxt* pGambleTxt = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "code", TXTFIELD_RAW, 0, 0, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	pGambleTxt = (D2GambleTxt*)DATATBLS_CompileTxt(pMemPool, "gamble", pTbl, &gpDataTables.pGambleDataTables.nGambleTxtRecordCount, sizeof(D2GambleTxt));

	if (gpDataTables.pGambleDataTables.nGambleTxtRecordCount)
	{
		gpDataTables.pGambleDataTables.pGambleSelection = (DWORD*)FOG_AllocServerMemory(NULL, sizeof(DWORD) * gpDataTables.pGambleDataTables.nGambleTxtRecordCount, __FILE__, __LINE__, 0);
		for (int i = 0; i < gpDataTables.pGambleDataTables.nGambleTxtRecordCount; ++i)
		{
			nItemId = FOG_GetLinkIndex(gpDataTables.pItemsLinker, pGambleTxt[i].dwItemCode, 0);
			D2_ASSERT(nItemId >= 0 && &gpDataTables.pItemDataTables.pItemsTxt[nItemId]);
			pGambleTxt[i].nItemId = nItemId;
			pGambleTxt[i].nLevel = gpDataTables.pItemDataTables.pItemsTxt[nItemId].nLevel;
		}
		qsort(pGambleTxt, gpDataTables.pGambleDataTables.nGambleTxtRecordCount, sizeof(D2GambleTxt), DATATBLS_CompareGambleTxtRecords);
	}
	else
	{
		gpDataTables.pGambleDataTables.pGambleSelection = NULL;
	}

	for (int i = 0; i < gpDataTables.pGambleDataTables.nGambleTxtRecordCount; ++i)
	{
		gpDataTables.pGambleDataTables.pGambleSelection[i] = pGambleTxt[i].nItemId;
	}

	gpDataTables.pGambleDataTables.pGambleChooseLimit[0] = 2;

	for (int i = 1; i < ARRAY_SIZE(gpDataTables.pGambleDataTables.pGambleChooseLimit); ++i)
	{
		gpDataTables.pGambleDataTables.pGambleChooseLimit[i] = gpDataTables.pGambleDataTables.nGambleTxtRecordCount;

		if (gpDataTables.pGambleDataTables.nGambleTxtRecordCount > 0)
		{
			nCounter = 0;
			while (nCounter < gpDataTables.pGambleDataTables.nGambleTxtRecordCount && i >= pGambleTxt[nCounter].nLevel)
			{
				++nCounter;
			}

			if (nCounter < gpDataTables.pGambleDataTables.nGambleTxtRecordCount)
			{
				gpDataTables.pGambleDataTables.pGambleChooseLimit[i] = nCounter;
			}
		}
	}

	DATATBLS_UnloadBin(pGambleTxt);
}

//D2Common.0x6FD5D790
int __cdecl DATATBLS_CompareGambleTxtRecords(const void* pRecord1, const void* pRecord2)
{
	D2GambleTxt* pElem1 = (D2GambleTxt*)pRecord1;
	D2GambleTxt* pElem2 = (D2GambleTxt*)pRecord2;

	if (pElem1->nLevel > pElem2->nLevel)
	{
		return 1;
	}
	else if (pElem1->nLevel == pElem2->nLevel)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

//D2Common.0x6FD5D7B0
void __fastcall DATATBLS_UnloadGambleTxt()
{
	if (gpDataTables.pGambleDataTables.pGambleSelection)
	{
		FOG_FreeServerMemory(NULL, gpDataTables.pGambleDataTables.pGambleSelection, __FILE__, __LINE__, 0);
		gpDataTables.pGambleDataTables.pGambleSelection = NULL;
		gpDataTables.pGambleDataTables.nGambleTxtRecordCount = 0;
	}
}

//D2Common.0x6FD5D7F0 (#10671)
D2GambleDataTbl* __fastcall DATATBLS_GetGambleDataTables()
{
	return &gpDataTables.pGambleDataTables;
}

//D2Common.0x6FD5D800
BOOL __fastcall DATATBLS_CheckNestedItemTypes(int nItemType1, int nItemType2)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	int nItemType = 0;
	int nIndex = 0;
	int nParentItemTypes[129] = {};

	if (nItemType2 <= 0)
	{
		return TRUE;
	}

	if (nItemType1 > 0 && nItemType1 < gpDataTables.nItemTypesTxtRecordCount)
	{
		nParentItemTypes[1] = nItemType1;
		nIndex = 1;

		while (1)
		{
			nItemType = nParentItemTypes[nIndex];
			--nIndex;

			if (nItemType2 == nItemType)
			{
				return TRUE;
			}

			if (nItemType >= gpDataTables.nItemTypesTxtRecordCount)
			{
				FOG_WriteToLogFile("Invalid item type at line %d of file %s", __LINE__, __FILE__);
				return FALSE;
			}

			if (nIndex > 124)//TODO: Limit of nParentItemTypes? Arbitrary? Only 124 ItemTypes possible?
			{
				return FALSE;
			}

			pItemTypesTxtRecord = &gpDataTables.pItemTypesTxt[nItemType];

			if (pItemTypesTxtRecord->nEquiv1 > 0)
			{
				++nIndex;
				nParentItemTypes[nIndex] = pItemTypesTxtRecord->nEquiv1;

				if (pItemTypesTxtRecord->nEquiv2)
				{
					++nIndex;
					nParentItemTypes[nIndex] = pItemTypesTxtRecord->nEquiv2;
				}
			}

			if (!nIndex)
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD5D8C0
void __fastcall DATATBLS_LoadItemTypesTxt(void* pMemPool)
{
	DWORD* pItemTypesNest = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &gpDataTables.pItemTypesLinker },
		{ "equiv1", TXTFIELD_CODETOWORD, 0, TXTFIELD_BYTE, &gpDataTables.pItemTypesLinker },
		{ "equiv2", TXTFIELD_CODETOWORD, 0, 6, &gpDataTables.pItemTypesLinker },
		{ "repair", TXTFIELD_BYTE, 0, 8, NULL },
		{ "body", TXTFIELD_BYTE, 0, 9, NULL },
		{ "bodyloc1", TXTFIELD_CODETOBYTE, 0, 10, &gpDataTables.pBodyLocsLinker },
		{ "bodyloc2", TXTFIELD_CODETOBYTE, 0, 11, &gpDataTables.pBodyLocsLinker },
		{ "shoots", TXTFIELD_CODETOWORD, 0, 12, &gpDataTables.pItemTypesLinker },
		{ "quiver", TXTFIELD_CODETOWORD, 0, 14, &gpDataTables.pItemTypesLinker },
		{ "throwable", TXTFIELD_BYTE, 0, 16, NULL },
		{ "reload", TXTFIELD_BYTE, 0, 17, NULL },
		{ "reequip", TXTFIELD_BYTE, 0, 18, NULL },
		{ "autostack", TXTFIELD_BYTE, 0, 19, NULL },
		{ "magic", TXTFIELD_BYTE, 0, 20, NULL },
		{ "rare", TXTFIELD_BYTE, 0, 21, NULL },
		{ "normal", TXTFIELD_BYTE, 0, 22, NULL },
		{ "charm", TXTFIELD_BYTE, 0, 23, NULL },
		{ "gem", TXTFIELD_BYTE, 0, 24, NULL },
		{ "beltable", TXTFIELD_BYTE, 0, 25, NULL },
		{ "maxsock1", TXTFIELD_BYTE, 0, 26, NULL },
		{ "maxsock25", TXTFIELD_BYTE, 0, 27, NULL },
		{ "maxsock40", TXTFIELD_BYTE, 0, 28, NULL },
		{ "treasureclass", TXTFIELD_BYTE, 0, 29, NULL },
		{ "rarity", TXTFIELD_BYTE, 0, 30, NULL },
		{ "staffmods", TXTFIELD_CODETOBYTE, 0, 31, &gpDataTables.pPlayerClassLinker },
		{ "costformula", TXTFIELD_BYTE, 0, 32, NULL },
		{ "class", TXTFIELD_CODETOBYTE, 0, 33, &gpDataTables.pPlayerClassLinker },
		{ "storepage", TXTFIELD_CODETOBYTE, 0, 34, &gpDataTables.pStorePageLinker },
		{ "varinvgfx", TXTFIELD_BYTE, 0, 35, NULL },
		{ "invgfx1", TXTFIELD_ASCII, 31, 36, NULL },
		{ "invgfx2", TXTFIELD_ASCII, 31, 68, NULL },
		{ "invgfx3", TXTFIELD_ASCII, 31, 100, NULL },
		{ "invgfx4", TXTFIELD_ASCII, 31, 132, NULL },
		{ "invgfx5", TXTFIELD_ASCII, 31, 164, NULL },
		{ "invgfx6", TXTFIELD_ASCII, 31, 196, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pItemTypesLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pItemTypesTxt = (D2ItemTypesTxt*)DATATBLS_CompileTxt(pMemPool, "itemtypes", pTbl, &gpDataTables.nItemTypesTxtRecordCount, sizeof(D2ItemTypesTxt));

	if (!gpDataTables.bCompileTxt && dword_6FDD6A24)
	{
		for (int i = 0; i < gpDataTables.nItemTypesTxtRecordCount; ++i)
		{
			FOG_10215(gpDataTables.pItemTypesLinker, *(DWORD*)&gpDataTables.pItemTypesTxt[i].szCode[0]);
		}
	}

	gpDataTables.nItemTypesIndex = (gpDataTables.nItemTypesTxtRecordCount + 31) / 32;
	gpDataTables.pItemTypesNest = (DWORD*)FOG_AllocServerMemory(NULL, sizeof(DWORD) * gpDataTables.nItemTypesTxtRecordCount * gpDataTables.nItemTypesIndex, __FILE__, __LINE__, 0);
	memset(gpDataTables.pItemTypesNest, 0x00, sizeof(DWORD) * gpDataTables.nItemTypesTxtRecordCount * gpDataTables.nItemTypesIndex);

	for (int i = 0; i < gpDataTables.nItemTypesTxtRecordCount; ++i)
	{
		pItemTypesNest = &gpDataTables.pItemTypesNest[gpDataTables.nItemTypesIndex * i];

		for (int j = 0; j < gpDataTables.nItemTypesTxtRecordCount; ++j)
		{
			if (DATATBLS_CheckNestedItemTypes(i, j))
			{
				pItemTypesNest[j >> 5] |= gdwBitMasks[j & 31];
			}
		}
	}
}

//D2Common.0x6FD5DFE0
void __fastcall DATATBLS_UnloadItemTypesTxt()
{
	FOG_FreeServerMemory(NULL, gpDataTables.pItemTypesNest, __FILE__, __LINE__, 0);
	DATATBLS_UnloadBin(gpDataTables.pItemTypesTxt);
	FOG_FreeLinker(gpDataTables.pItemTypesLinker);
}

//D2Common.0x6FD5E020
void __fastcall DATATBLS_LoadRunesTxt(void* pMemPool)
{
	void* pFileHandle = NULL;
	char szPath[100] = {};
	wchar_t* pUnicode = NULL;
	D2BinFieldStrc pTbl[] =
	{
		{ "name", TXTFIELD_ASCII, 63, 0, NULL },
		{ "rune name", TXTFIELD_ASCII, 63, 64, NULL },
		{ "complete", TXTFIELD_BYTE, 0, 128, NULL },
		{ "server", TXTFIELD_BYTE, 0, 129, NULL },
		{ "itype1", TXTFIELD_CODETOWORD, 0, 134, &gpDataTables.pItemTypesLinker },
		{ "itype2", TXTFIELD_CODETOWORD, 0, 136, &gpDataTables.pItemTypesLinker },
		{ "itype3", TXTFIELD_CODETOWORD, 0, 138, &gpDataTables.pItemTypesLinker },
		{ "itype4", TXTFIELD_CODETOWORD, 0, 140, &gpDataTables.pItemTypesLinker },
		{ "itype5", TXTFIELD_CODETOWORD, 0, 142, &gpDataTables.pItemTypesLinker },
		{ "itype6", TXTFIELD_CODETOWORD, 0, 144, &gpDataTables.pItemTypesLinker },
		{ "etype1", TXTFIELD_CODETOWORD, 0, 146, &gpDataTables.pItemTypesLinker },
		{ "etype2", TXTFIELD_CODETOWORD, 0, 148, &gpDataTables.pItemTypesLinker },
		{ "etype3", TXTFIELD_CODETOWORD, 0, 150, &gpDataTables.pItemTypesLinker },
		{ "rune1", TXTFIELD_UNKNOWN3, 0, 152, &gpDataTables.pItemsLinker },
		{ "rune2", TXTFIELD_UNKNOWN3, 0, 156, &gpDataTables.pItemsLinker },
		{ "rune3", TXTFIELD_UNKNOWN3, 0, 160, &gpDataTables.pItemsLinker },
		{ "rune4", TXTFIELD_UNKNOWN3, 0, 164, &gpDataTables.pItemsLinker },
		{ "rune5", TXTFIELD_UNKNOWN3, 0, 168, &gpDataTables.pItemsLinker },
		{ "rune6", TXTFIELD_UNKNOWN3, 0, 172, &gpDataTables.pItemsLinker },
		{ "t1code1", TXTFIELD_NAMETODWORD, 0, 176, &gpDataTables.pPropertiesLinker },
		{ "t1param1", TXTFIELD_CALCTODWORD, 0, 180, DATATBLS_ItemParamLinker },
		{ "t1min1", TXTFIELD_DWORD, 0, 184, NULL },
		{ "t1max1", TXTFIELD_DWORD, 0, 188, NULL },
		{ "t1code2", TXTFIELD_NAMETODWORD, 0, 192, &gpDataTables.pPropertiesLinker },
		{ "t1param2", TXTFIELD_CALCTODWORD, 0, 196, DATATBLS_ItemParamLinker },
		{ "t1min2", TXTFIELD_DWORD, 0, 200, NULL },
		{ "t1max2", TXTFIELD_DWORD, 0, 204, NULL },
		{ "t1code3", TXTFIELD_NAMETODWORD, 0, 208, &gpDataTables.pPropertiesLinker },
		{ "t1param3", TXTFIELD_CALCTODWORD, 0, 212, DATATBLS_ItemParamLinker },
		{ "t1min3", TXTFIELD_DWORD, 0, 216, NULL },
		{ "t1max3", TXTFIELD_DWORD, 0, 220, NULL },
		{ "t1code4", TXTFIELD_NAMETODWORD, 0, 224, &gpDataTables.pPropertiesLinker },
		{ "t1param4", TXTFIELD_CALCTODWORD, 0, 228, DATATBLS_ItemParamLinker },
		{ "t1min4", TXTFIELD_DWORD, 0, 232, NULL },
		{ "t1max4", TXTFIELD_DWORD, 0, 236, NULL },
		{ "t1code5", TXTFIELD_NAMETODWORD, 0, 240, &gpDataTables.pPropertiesLinker },
		{ "t1param5", TXTFIELD_CALCTODWORD, 0, 244, DATATBLS_ItemParamLinker },
		{ "t1min5", TXTFIELD_DWORD, 0, 248, NULL },
		{ "t1max5", TXTFIELD_DWORD, 0, 252, NULL },
		{ "t1code6", TXTFIELD_NAMETODWORD, 0, 256, &gpDataTables.pPropertiesLinker },
		{ "t1param6", TXTFIELD_CALCTODWORD, 0, 260, DATATBLS_ItemParamLinker },
		{ "t1min6", TXTFIELD_DWORD, 0, 264, NULL },
		{ "t1max6", TXTFIELD_DWORD, 0, 268, NULL },
		{ "t1code7", TXTFIELD_NAMETODWORD, 0, 272, &gpDataTables.pPropertiesLinker },
		{ "t1param7", TXTFIELD_CALCTODWORD, 0, 276, DATATBLS_ItemParamLinker },
		{ "t1min7", TXTFIELD_DWORD, 0, 280, NULL },
		{ "t1max7", TXTFIELD_DWORD, 0, 284, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pRunesLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);

	wsprintfA(szPath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", "runessrv", ".txt");
	if (DATATBLS_CheckIfFileExists(pMemPool, szPath, &pFileHandle, 1))
	{
		DATATBLS_CloseFileInMPQ(pMemPool, pFileHandle);
		FOG_10025("Found runessrv.txt in archive - This file should only be in server builds.", __FILE__, __LINE__);
	}

	wsprintfA(szPath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", "runessrv", ".bin");
	if (DATATBLS_CheckIfFileExists(pMemPool, szPath, &pFileHandle, 1))
	{
		DATATBLS_CloseFileInMPQ(pMemPool, pFileHandle);
		FOG_10025("Found runessrv.bin in archive - This file should only be in server builds.", __FILE__, __LINE__);
	}

	wsprintfA(szPath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", "runessrv", ".xls");
	if (DATATBLS_CheckIfFileExists(pMemPool, szPath, &pFileHandle, 1))
	{
		DATATBLS_CloseFileInMPQ(pMemPool, pFileHandle);
		FOG_10025("Found runessrv.xls in archive - This file should only be in server builds.", __FILE__, __LINE__);
	}

	gpDataTables.pRuneDataTables.pRunesTxt = (D2RunesTxt*)DATATBLS_CompileTxt(pMemPool, "runes", pTbl, &gpDataTables.pRuneDataTables.nRunesTxtRecordCount, sizeof(D2RunesTxt));

	for (int i = 0; i < gpDataTables.pRuneDataTables.nRunesTxtRecordCount; ++i)
	{
		gpDataTables.pRuneDataTables.pRunesTxt[i].wStringId = D2LANG_GetTblIndex(gpDataTables.pRuneDataTables.pRunesTxt[i].szName, &pUnicode);
	}
}

//D2Common.0x6FD5E9C0
void __fastcall DATATBLS_UnloadRunesTxt()
{
	DATATBLS_UnloadBin(gpDataTables.pRuneDataTables.pRunesTxt);
	FOG_FreeLinker(gpDataTables.pRunesLinker);
}

//D2Common.0x6FD5E9E0 (#10619)
D2RuneDataTbl* __fastcall DATATBLS_GetRuneDataTables()
{
	return &gpDataTables.pRuneDataTables;
}

//D2Common.0x6FD5E9F0 (#10621)
void __stdcall DATATBLS_AddOrChangeRunesTxtRecord(int nRecordId, D2RunesTxt* pRecord)
{
	D2RunesTxt* pRunesTxt = NULL;
	D2RunesTxt* pTmp = NULL;

	int nSize = 0;

	if (nRecordId >= gpDataTables.pRuneDataTables.nRunesTxtRecordCount)
	{
		pRunesTxt = gpDataTables.pRuneDataTables.pRunesTxt;
		nSize = sizeof(D2RunesTxt) * (nRecordId + 1);

		if (dword_6FDD6A24)
		{
			pRunesTxt = (D2RunesTxt*)((char*)gpDataTables.pRuneDataTables.pRunesTxt - 4);
			nSize += 4;
		}

		pTmp = (D2RunesTxt*)FOG_ReallocServerMemory(NULL, pRunesTxt, nSize, __FILE__, __LINE__, 0);

		if (dword_6FDD6A24)
		{
			*(DWORD*)pTmp = nRecordId + 1;
			pTmp = (D2RunesTxt*)((char*)pTmp + 4);
		}
		gpDataTables.pRuneDataTables.pRunesTxt = pTmp;

		memset(&pTmp[gpDataTables.pRuneDataTables.nRunesTxtRecordCount], 0x00, sizeof(D2RunesTxt) * (nRecordId - gpDataTables.pRuneDataTables.nRunesTxtRecordCount));
		gpDataTables.pRuneDataTables.nRunesTxtRecordCount = nRecordId + 1;
	}

	memcpy(&gpDataTables.pRuneDataTables.pRunesTxt[nRecordId], pRecord, sizeof(gpDataTables.pRuneDataTables.pRunesTxt[nRecordId]));
}

//D2Common.0x6FD5EAA0 (#10620)
D2RunesTxt* __stdcall DATATBLS_GetRunesTxtRecord(int nRunewordId)
{
	D2_ASSERT(nRunewordId > 0 || nRunewordId < gpDataTables.pRuneDataTables.nRunesTxtRecordCount);
	return &gpDataTables.pRuneDataTables.pRunesTxt[nRunewordId];
}
