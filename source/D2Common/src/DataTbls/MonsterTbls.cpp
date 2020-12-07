#include "D2DataTbls.h"

#include "D2Items.h"
#include "D2Seed.h"


//Inlined in some functions
DWORD __fastcall DATATBLS_StringToCode(char* szText)
{
	char dwCode[4] = {};

	if (szText[0])
	{
		dwCode[0] = szText[0];
		if (szText[1])
		{
			dwCode[1] = szText[1];
			if (szText[2])
			{
				dwCode[2] = szText[2];
				if (szText[3])
				{
					dwCode[3] = szText[3];
				}
				else
				{
					dwCode[3] = ' ';
				}
			}
			else
			{
				dwCode[2] = ' ';
				dwCode[3] = ' ';
			}
		}
		else
		{
			dwCode[1] = ' ';
			dwCode[2] = ' ';
			dwCode[3] = ' ';
		}
	}
	else
	{
		dwCode[0] = ' ';
		dwCode[1] = ' ';
		dwCode[2] = ' ';
		dwCode[3] = ' ';
	}

	return *(DWORD*)dwCode;
}


//D2Common.0x6FD64C40 (#10592)
int __stdcall DATATBLS_GetMonsterChainInfo(int nMonsterId, int* pMaxChainId, int* pChainId)
{
	D2MonStatsTxt* pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);

	if (pMonStatsTxtRecord)
	{
		if (pMaxChainId)
		{
			*pMaxChainId = pMonStatsTxtRecord->nMaxChainId;
		}

		if (pChainId)
		{
			*pChainId = pMonStatsTxtRecord->nChainId;
		}

		return pMonStatsTxtRecord->nChainId;
	}
	else
	{
		if (pMaxChainId)
		{
			*pMaxChainId = 0;
		}

		if (pChainId)
		{
			*pChainId = 0;
		}

		return 0;
	}
}

//D2Common.0x6FD64CD0
void __fastcall DATATBLS_MonStatsSkillModeLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	char* pTmp = NULL;
	unsigned int nInverseLength = 0;
	char szCurrentChar = 0;
	char nMode = 0;
	char szMode[4] = {};

	pMonStatsTxtRecord = (D2MonStatsTxt*)pRecord;
	if (pRecord && nOffset >= 0 && nOffset < 8)
	{
		pMonStatsTxtRecord->nSkillMode[nOffset] = 0;
		pMonStatsTxtRecord->nSequence[nOffset] = -1;

		if (pMonStatsTxtRecord->nSkill[nOffset] >= 0 && pSrc)
		{
			nInverseLength = 0xFFFFFFFF;
			pTmp = pSrc;
			do
			{
				if (!nInverseLength)
				{
					break;
				}

				szCurrentChar = *pTmp;
				++pTmp;
				--nInverseLength;
			}
			while (szCurrentChar);

			if (~nInverseLength - 1 > 3)
			{
				pMonStatsTxtRecord->nSkillMode[nOffset] = MONMODE_SEQUENCE;
				pMonStatsTxtRecord->nSequence[nOffset] = FOG_GetRowFromTxt(gpDataTables.pMonSeqLinker, pSrc, 1);
				return;
			}

			pTmp = pSrc;
			do
			{
				szCurrentChar = *pTmp;
				szMode[pTmp - pSrc] = szCurrentChar;
				++pTmp;
			}
			while (szCurrentChar);

			nMode = FOG_GetLinkIndex(gpDataTables.pMonModeLinker, DATATBLS_StringToCode(szMode), 1);
			pMonStatsTxtRecord->nSkillMode[nOffset] = nMode;

			if (nMode < 0 || nMode == MONMODE_SEQUENCE)
			{
				pMonStatsTxtRecord->nSkillMode[nOffset] = MONMODE_SEQUENCE;
				pMonStatsTxtRecord->nSequence[nOffset] = FOG_GetRowFromTxt(gpDataTables.pMonSeqLinker, pSrc, 1);
			}
		}
	}
}

//D2Common.0x6FD64DF0
void __fastcall DATATBLS_LoadMonStatsTxt(void* pMemPool)
{
	long long v21 = 0; //TODO: Check / Remove

	char* szMonsterName = NULL;
	int nNextInClass = 0;
	int nVelocity = 0;
	int nCounter = 0;
	int nChainId = 0;
	int nBaseId = 0;
	int nRun = 0;
	int nId = 0;
	bool bFoundInChain = false;

	D2BinFieldStrc pTbl[] =
	{
		{ "Id", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pMonStatsLinker },
		{ "BaseId", TXTFIELD_NAMETOWORD, 0, 2, &gpDataTables.pMonStatsLinker },
		{ "NextInClass", TXTFIELD_NAMETOWORD, 0, 4, &gpDataTables.pMonStatsLinker },
		{ "NameStr", TXTFIELD_KEYTOWORD, 0, 6, DATATBLS_GetStringIdFromReferenceString },
		{ "DescStr", TXTFIELD_KEYTOWORD, 0, 8, DATATBLS_GetStringIdFromReferenceString },
		{ "Code", TXTFIELD_RAW, 0, 16, NULL },
		{ "TransLvl", TXTFIELD_BYTE, 0, 77, NULL },
		{ "MonSound", TXTFIELD_NAMETOWORD, 0, 20, &gpDataTables.pMonSoundsLinker },
		{ "UMonSound", TXTFIELD_NAMETOWORD, 0, 22, &gpDataTables.pMonSoundsLinker },
		{ "MonStatsEx", TXTFIELD_NAMETOWORD, 0, 24, &gpDataTables.pMonStats2Linker },
		{ "MonType", TXTFIELD_NAMETOWORD, 0, 28, &gpDataTables.pMonTypeLinker },
		{ "MonProp", TXTFIELD_NAMETOWORD, 0, 26, &gpDataTables.pMonPropLinker },
		{ "AI", TXTFIELD_NAMETOWORD, 0, 30, &gpDataTables.pMonAiLinker },
		{ "spawn", TXTFIELD_NAMETOWORD, 0, 32, &gpDataTables.pMonStatsLinker },
		{ "spawnx", TXTFIELD_BYTE, 0, 34, NULL },
		{ "spawny", TXTFIELD_BYTE, 0, 35, NULL },
		{ "spawnmode", TXTFIELD_CODETOBYTE, 0, 36, &gpDataTables.pMonModeLinker },
		{ "minion1", TXTFIELD_NAMETOWORD, 0, 38, &gpDataTables.pMonStatsLinker },
		{ "minion2", TXTFIELD_NAMETOWORD, 0, 40, &gpDataTables.pMonStatsLinker },
		{ "PartyMin", TXTFIELD_BYTE, 0, 44, NULL },
		{ "PartyMax", TXTFIELD_BYTE, 0, 45, NULL },
		{ "Rarity", TXTFIELD_BYTE, 0, 46, NULL },
		{ "MinGrp", TXTFIELD_BYTE, 0, 47, NULL },
		{ "MaxGrp", TXTFIELD_BYTE, 0, 48, NULL },
		{ "sparsePopulate", TXTFIELD_BYTE, 0, 49, NULL },
		{ "Velocity", TXTFIELD_WORD, 0, 50, NULL },
		{ "Run", TXTFIELD_WORD, 0, 52, NULL },
		{ "Align", TXTFIELD_BYTE, 0, 76, NULL },
		{ "enabled", TXTFIELD_BIT, 25, 12, NULL },
		{ "rangedtype", TXTFIELD_BIT, 28, 12, NULL },
		{ "placespawn", TXTFIELD_BIT, 23, 12, NULL },
		{ "isSpawn", TXTFIELD_BIT, 0, 12, NULL },
		{ "isMelee", TXTFIELD_BIT, 1, 12, NULL },
		{ "noRatio", TXTFIELD_BIT, 2, 12, NULL },
		{ "SetBoss", TXTFIELD_BIT, 4, 12, NULL },
		{ "BossXfer", TXTFIELD_BIT, 5, 12, NULL },
		{ "boss", TXTFIELD_BIT, 6, 12, NULL },
		{ "primeevil", TXTFIELD_BIT, 7, 12, NULL },
		{ "opendoors", TXTFIELD_BIT, 3, 12, NULL },
		{ "npc", TXTFIELD_BIT, 8, 12, NULL },
		{ "interact", TXTFIELD_BIT, 9, 12, NULL },
		{ "inventory", TXTFIELD_BIT, 24, 12, NULL },
		{ "inTown", TXTFIELD_BIT, 10, 12, NULL },
		{ "lUndead", TXTFIELD_BIT, 11, 12, NULL },
		{ "hUndead", TXTFIELD_BIT, 12, 12, NULL },
		{ "demon", TXTFIELD_BIT, 13, 12, NULL },
		{ "flying", TXTFIELD_BIT, 14, 12, NULL },
		{ "killable", TXTFIELD_BIT, 15, 12, NULL },
		{ "switchai", TXTFIELD_BIT, 16, 12, NULL },
		{ "noaura", TXTFIELD_BIT, 27, 12, NULL },
		{ "nomultishot", TXTFIELD_BIT, 17, 12, NULL },
		{ "neverCount", TXTFIELD_BIT, 18, 12, NULL },
		{ "petIgnore", TXTFIELD_BIT, 19, 12, NULL },
		{ "deathDmg", TXTFIELD_BIT, 20, 12, NULL },
		{ "genericSpawn", TXTFIELD_BIT, 21, 12, NULL },
		{ "zoo", TXTFIELD_BIT, 22, 12, NULL },
		{ "MissA1", TXTFIELD_NAMETOWORD, 0, 58, &gpDataTables.pMissilesLinker },
		{ "MissA2", TXTFIELD_NAMETOWORD, 0, 60, &gpDataTables.pMissilesLinker },
		{ "MissS1", TXTFIELD_NAMETOWORD, 0, 62, &gpDataTables.pMissilesLinker },
		{ "MissS2", TXTFIELD_NAMETOWORD, 0, 64, &gpDataTables.pMissilesLinker },
		{ "MissS3", TXTFIELD_NAMETOWORD, 0, 66, &gpDataTables.pMissilesLinker },
		{ "MissS4", TXTFIELD_NAMETOWORD, 0, 68, &gpDataTables.pMissilesLinker },
		{ "MissC", TXTFIELD_NAMETOWORD, 0, 70, &gpDataTables.pMissilesLinker },
		{ "MissSQ", TXTFIELD_NAMETOWORD, 0, 72, &gpDataTables.pMissilesLinker },
		{ "TreasureClass1", TXTFIELD_NAMETOWORD, 0, 134, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass2", TXTFIELD_NAMETOWORD, 0, 136, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass3", TXTFIELD_NAMETOWORD, 0, 138, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass4", TXTFIELD_NAMETOWORD, 0, 140, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass1(N)", TXTFIELD_NAMETOWORD, 0, 142, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass2(N)", TXTFIELD_NAMETOWORD, 0, 144, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass3(N)", TXTFIELD_NAMETOWORD, 0, 146, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass4(N)", TXTFIELD_NAMETOWORD, 0, 148, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass1(H)", TXTFIELD_NAMETOWORD, 0, 150, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass2(H)", TXTFIELD_NAMETOWORD, 0, 152, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass3(H)", TXTFIELD_NAMETOWORD, 0, 154, &gpDataTables.pTreasureClassExLinker },
		{ "TreasureClass4(H)", TXTFIELD_NAMETOWORD, 0, 156, &gpDataTables.pTreasureClassExLinker },
		{ "TCQuestId", TXTFIELD_BYTE, 0, 158, NULL },
		{ "TCQuestCP", TXTFIELD_BYTE, 0, 159, NULL },
		{ "threat", TXTFIELD_BYTE, 0, 78, NULL },
		{ "aidel", TXTFIELD_BYTE, 0, 79, NULL },
		{ "aidel(N)", TXTFIELD_BYTE, 0, 80, NULL },
		{ "aidel(H)", TXTFIELD_BYTE, 0, 81, NULL },
		{ "aidist", TXTFIELD_BYTE, 0, 82, NULL },
		{ "aidist(N)", TXTFIELD_BYTE, 0, 83, NULL },
		{ "aidist(H)", TXTFIELD_BYTE, 0, 84, NULL },
		{ "aip1", TXTFIELD_WORD, 0, 86, NULL },
		{ "aip1(N)", TXTFIELD_WORD, 0, 88, NULL },
		{ "aip1(H)", TXTFIELD_WORD, 0, 90, NULL },
		{ "aip2", TXTFIELD_WORD, 0, 92, NULL },
		{ "aip2(N)", TXTFIELD_WORD, 0, 94, NULL },
		{ "aip2(H)", TXTFIELD_WORD, 0, 96, NULL },
		{ "aip3", TXTFIELD_WORD, 0, 98, NULL },
		{ "aip3(N)", TXTFIELD_WORD, 0, 100, NULL },
		{ "aip3(H)", TXTFIELD_WORD, 0, 102, NULL },
		{ "aip4", TXTFIELD_WORD, 0, 104, NULL },
		{ "aip4(N)", TXTFIELD_WORD, 0, 106, NULL },
		{ "aip4(H)", TXTFIELD_WORD, 0, 108, NULL },
		{ "aip5", TXTFIELD_WORD, 0, 110, NULL },
		{ "aip5(N)", TXTFIELD_WORD, 0, 112, NULL },
		{ "aip5(H)", TXTFIELD_WORD, 0, 114, NULL },
		{ "aip6", TXTFIELD_WORD, 0, 116, NULL },
		{ "aip6(N)", TXTFIELD_WORD, 0, 118, NULL },
		{ "aip6(H)", TXTFIELD_WORD, 0, 120, NULL },
		{ "aip7", TXTFIELD_WORD, 0, 122, NULL },
		{ "aip7(N)", TXTFIELD_WORD, 0, 124, NULL },
		{ "aip7(H)", TXTFIELD_WORD, 0, 126, NULL },
		{ "aip8", TXTFIELD_WORD, 0, 128, NULL },
		{ "aip8(N)", TXTFIELD_WORD, 0, 130, NULL },
		{ "aip8(H)", TXTFIELD_WORD, 0, 132, NULL },
		{ "Level", TXTFIELD_WORD, 0, 170, NULL },
		{ "Level(N)", TXTFIELD_WORD, 0, 172, NULL },
		{ "Level(H)", TXTFIELD_WORD, 0, 174, NULL },
		{ "SkillDamage", TXTFIELD_NAMETOWORD, 0, 168, &gpDataTables.pSkillsLinker },
		{ "NoShldBlock", TXTFIELD_BIT, 26, 12, NULL },
		{ "Drain", TXTFIELD_BYTE, 0, 160, NULL },
		{ "Drain(N)", TXTFIELD_BYTE, 0, 161, NULL },
		{ "Drain(H)", TXTFIELD_BYTE, 0, 162, NULL },
		{ "ToBlock", TXTFIELD_BYTE, 0, 163, NULL },
		{ "ToBlock(N)", TXTFIELD_BYTE, 0, 164, NULL },
		{ "ToBlock(H)", TXTFIELD_BYTE, 0, 165, NULL },
		{ "Crit", TXTFIELD_BYTE, 0, 166, NULL },
		{ "MinHP", TXTFIELD_WORD, 0, 176, NULL },
		{ "MinHP(N)", TXTFIELD_WORD, 0, 178, NULL },
		{ "MinHP(H)", TXTFIELD_WORD, 0, 180, NULL },
		{ "MaxHP", TXTFIELD_WORD, 0, 182, NULL },
		{ "MaxHP(N)", TXTFIELD_WORD, 0, 184, NULL },
		{ "MaxHP(H)", TXTFIELD_WORD, 0, 186, NULL },
		{ "AC", TXTFIELD_WORD, 0, 188, NULL },
		{ "AC(N)", TXTFIELD_WORD, 0, 190, NULL },
		{ "AC(H)", TXTFIELD_WORD, 0, 192, NULL },
		{ "Exp", TXTFIELD_WORD, 0, 212, NULL },
		{ "Exp(N)", TXTFIELD_WORD, 0, 214, NULL },
		{ "Exp(H)", TXTFIELD_WORD, 0, 216, NULL },
		{ "A1TH", TXTFIELD_WORD, 0, 194, NULL },
		{ "A1TH(N)", TXTFIELD_WORD, 0, 196, NULL },
		{ "A1TH(H)", TXTFIELD_WORD, 0, 198, NULL },
		{ "A1MinD", TXTFIELD_WORD, 0, 218, NULL },
		{ "A1MinD(N)", TXTFIELD_WORD, 0, 220, NULL },
		{ "A1MinD(H)", TXTFIELD_WORD, 0, 222, NULL },
		{ "A1MaxD", TXTFIELD_WORD, 0, 224, NULL },
		{ "A1MaxD(N)", TXTFIELD_WORD, 0, 226, NULL },
		{ "A1MaxD(H)", TXTFIELD_WORD, 0, 228, NULL },
		{ "A2TH", TXTFIELD_WORD, 0, 200, NULL },
		{ "A2TH(N)", TXTFIELD_WORD, 0, 202, NULL },
		{ "A2TH(H)", TXTFIELD_WORD, 0, 204, NULL },
		{ "A2MinD", TXTFIELD_WORD, 0, 230, NULL },
		{ "A2MinD(N)", TXTFIELD_WORD, 0, 232, NULL },
		{ "A2MinD(H)", TXTFIELD_WORD, 0, 234, NULL },
		{ "A2MaxD", TXTFIELD_WORD, 0, 236, NULL },
		{ "A2MaxD(N)", TXTFIELD_WORD, 0, 238, NULL },
		{ "A2MaxD(H)", TXTFIELD_WORD, 0, 240, NULL },
		{ "S1TH", TXTFIELD_WORD, 0, 206, NULL },
		{ "S1TH(N)", TXTFIELD_WORD, 0, 208, NULL },
		{ "S1TH(H)", TXTFIELD_WORD, 0, 210, NULL },
		{ "S1MinD", TXTFIELD_WORD, 0, 242, NULL },
		{ "S1MinD(N)", TXTFIELD_WORD, 0, 244, NULL },
		{ "S1MinD(H)", TXTFIELD_WORD, 0, 246, NULL },
		{ "S1MaxD", TXTFIELD_WORD, 0, 248, NULL },
		{ "S1MaxD(N)", TXTFIELD_WORD, 0, 250, NULL },
		{ "S1MaxD(H)", TXTFIELD_WORD, 0, 252, NULL },
		{ "El1Mode", TXTFIELD_CODETOBYTE, 0, 254, &gpDataTables.pMonModeLinker },
		{ "El1Type", TXTFIELD_CODETOBYTE, 0, 257, &gpDataTables.pElemTypesLinker },
		{ "El1Pct", TXTFIELD_BYTE, 0, 260, NULL },
		{ "El1Pct(N)", TXTFIELD_BYTE, 0, 261, NULL },
		{ "El1Pct(H)", TXTFIELD_BYTE, 0, 262, NULL },
		{ "El1MinD", TXTFIELD_WORD, 0, 270, NULL },
		{ "El1MinD(N)", TXTFIELD_WORD, 0, 272, NULL },
		{ "El1MinD(H)", TXTFIELD_WORD, 0, 274, NULL },
		{ "El1MaxD", TXTFIELD_WORD, 0, 288, NULL },
		{ "El1MaxD(N)", TXTFIELD_WORD, 0, 290, NULL },
		{ "El1MaxD(H)", TXTFIELD_WORD, 0, 292, NULL },
		{ "El1Dur", TXTFIELD_WORD, 0, 306, NULL },
		{ "El1Dur(N)", TXTFIELD_WORD, 0, 308, NULL },
		{ "El1Dur(H)", TXTFIELD_WORD, 0, 310, NULL },
		{ "El2Mode", TXTFIELD_CODETOBYTE, 0, 255, &gpDataTables.pMonModeLinker },
		{ "El2Type", TXTFIELD_CODETOBYTE, 0, 258, &gpDataTables.pElemTypesLinker },
		{ "El2Pct", TXTFIELD_BYTE, 0, 263, NULL },
		{ "El2Pct(N)", TXTFIELD_BYTE, 0, 264, NULL },
		{ "El2Pct(H)", TXTFIELD_BYTE, 0, 265, NULL },
		{ "El2MinD", TXTFIELD_WORD, 0, 276, NULL },
		{ "El2MinD(N)", TXTFIELD_WORD, 0, 278, NULL },
		{ "El2MinD(H)", TXTFIELD_WORD, 0, 280, NULL },
		{ "El2MaxD", TXTFIELD_WORD, 0, 294, NULL },
		{ "El2MaxD(N)", TXTFIELD_WORD, 0, 296, NULL },
		{ "El2MaxD(H)", TXTFIELD_WORD, 0, 298, NULL },
		{ "El2Dur", TXTFIELD_WORD, 0, 312, NULL },
		{ "El2Dur(N)", TXTFIELD_WORD, 0, 314, NULL },
		{ "El2Dur(H)", TXTFIELD_WORD, 0, 316, NULL },
		{ "El3Mode", TXTFIELD_CODETOBYTE, 0, 256, &gpDataTables.pMonModeLinker },
		{ "El3Type", TXTFIELD_CODETOBYTE, 0, 259, &gpDataTables.pElemTypesLinker },
		{ "El3Pct", TXTFIELD_BYTE, 0, 266, NULL },
		{ "El3Pct(N)", TXTFIELD_BYTE, 0, 267, NULL },
		{ "El3Pct(H)", TXTFIELD_BYTE, 0, 268, NULL },
		{ "El3MinD", TXTFIELD_WORD, 0, 282, NULL },
		{ "El3MinD(N)", TXTFIELD_WORD, 0, 284, NULL },
		{ "El3MinD(H)", TXTFIELD_WORD, 0, 286, NULL },
		{ "El3MaxD", TXTFIELD_WORD, 0, 300, NULL },
		{ "El3MaxD(N)", TXTFIELD_WORD, 0, 302, NULL },
		{ "El3MaxD(H)", TXTFIELD_WORD, 0, 304, NULL },
		{ "El3Dur", TXTFIELD_WORD, 0, 318, NULL },
		{ "El3Dur(N)", TXTFIELD_WORD, 0, 320, NULL },
		{ "El3Dur(H)", TXTFIELD_WORD, 0, 322, NULL },
		{ "ColdEffect", TXTFIELD_BYTE, 0, 360, NULL },
		{ "ColdEffect(N)", TXTFIELD_BYTE, 0, 361, NULL },
		{ "ColdEffect(H)", TXTFIELD_BYTE, 0, 362, NULL },
		{ "ResDm", TXTFIELD_WORD, 0, 324, NULL },
		{ "ResDm(N)", TXTFIELD_WORD, 0, 326, NULL },
		{ "ResDm(H)", TXTFIELD_WORD, 0, 328, NULL },
		{ "ResMa", TXTFIELD_WORD, 0, 330, NULL },
		{ "ResMa(N)", TXTFIELD_WORD, 0, 332, NULL },
		{ "ResMa(H)", TXTFIELD_WORD, 0, 334, NULL },
		{ "ResFi", TXTFIELD_WORD, 0, 336, NULL },
		{ "ResFi(N)", TXTFIELD_WORD, 0, 338, NULL },
		{ "ResFi(H)", TXTFIELD_WORD, 0, 340, NULL },
		{ "ResLi", TXTFIELD_WORD, 0, 342, NULL },
		{ "ResLi(N)", TXTFIELD_WORD, 0, 344, NULL },
		{ "ResLi(H)", TXTFIELD_WORD, 0, 346, NULL },
		{ "ResCo", TXTFIELD_WORD, 0, 348, NULL },
		{ "ResCo(N)", TXTFIELD_WORD, 0, 350, NULL },
		{ "ResCo(H)", TXTFIELD_WORD, 0, 352, NULL },
		{ "ResPo", TXTFIELD_WORD, 0, 354, NULL },
		{ "ResPo(N)", TXTFIELD_WORD, 0, 356, NULL },
		{ "ResPo(H)", TXTFIELD_WORD, 0, 358, NULL },
		{ "SendSkills", TXTFIELD_DWORD2, 0, 364, NULL },
		{ "Skill1", TXTFIELD_NAMETOWORD, 0, 368, &gpDataTables.pSkillsLinker },
		{ "Sk1mode", TXTFIELD_CUSTOMLINK, 0, 0, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk1lvl", TXTFIELD_BYTE, 0, 408, NULL },
		{ "Skill2", TXTFIELD_NAMETOWORD, 0, 370, &gpDataTables.pSkillsLinker },
		{ "Sk2mode", TXTFIELD_CUSTOMLINK, 0, 1, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk2lvl", TXTFIELD_BYTE, 0, 409, NULL },
		{ "Skill3", TXTFIELD_NAMETOWORD, 0, 372, &gpDataTables.pSkillsLinker },
		{ "Sk3mode", TXTFIELD_CUSTOMLINK, 0, 2, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk3lvl", TXTFIELD_BYTE, 0, 410, NULL },
		{ "Skill4", TXTFIELD_NAMETOWORD, 0, 374, &gpDataTables.pSkillsLinker },
		{ "Sk4mode", TXTFIELD_CUSTOMLINK, 0, 3, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk4lvl", TXTFIELD_BYTE, 0, 411, NULL },
		{ "Skill5", TXTFIELD_NAMETOWORD, 0, 376, &gpDataTables.pSkillsLinker },
		{ "Sk5mode", TXTFIELD_CUSTOMLINK, 0, 4, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk5lvl", TXTFIELD_BYTE, 0, 412, NULL },
		{ "Skill6", TXTFIELD_NAMETOWORD, 0, 378, &gpDataTables.pSkillsLinker },
		{ "Sk6mode", TXTFIELD_CUSTOMLINK, 0, 5, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk6lvl", TXTFIELD_BYTE, 0, 413, NULL },
		{ "Skill7", TXTFIELD_NAMETOWORD, 0, 380, &gpDataTables.pSkillsLinker },
		{ "Sk7mode", TXTFIELD_CUSTOMLINK, 0, 6, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk7lvl", TXTFIELD_BYTE, 0, 414, NULL },
		{ "Skill8", TXTFIELD_NAMETOWORD, 0, 382, &gpDataTables.pSkillsLinker },
		{ "Sk8mode", TXTFIELD_CUSTOMLINK, 0, 7, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk8lvl", TXTFIELD_BYTE, 0, 415, NULL },
		{ "DamageRegen", TXTFIELD_DWORD, 0, 416, NULL },
		{ "SplEndDeath", TXTFIELD_BYTE, 0, 420, NULL },
		{ "SplGetModeChart", TXTFIELD_BYTE, 0, 421, NULL },
		{ "SplEndGeneric", TXTFIELD_BYTE, 0, 422, NULL },
		{ "SplClientEnd", TXTFIELD_BYTE, 0, 423, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonStatsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pMonStatsTxt = (D2MonStatsTxt*)DATATBLS_CompileTxt(pMemPool, "monstats", pTbl, &gpDataTables.nMonStatsTxtRecordCount, sizeof(D2MonStatsTxt));

	D2_ASSERT(gpDataTables.nMonStatsTxtRecordCount < SHRT_MAX);

	if (gpDataTables.nMonStatsTxtRecordCount > 0)
	{
		nCounter = 0;
		do
		{
			bFoundInChain = false;

			nChainId = 0;
			nId = gpDataTables.pMonStatsTxt[nCounter].nBaseId;

			do
			{
				if (gpDataTables.pMonStatsTxt[nId].nBaseId != gpDataTables.pMonStatsTxt[nCounter].nBaseId && gpDataTables.bCompileTxt)
				{
					szMonsterName = FOG_10255(gpDataTables.pMonStatsLinker, gpDataTables.pMonStatsTxt[nId].nId, 0);
					FOG_WriteToLogFile("BaseId/NextInClass chain -- unexpected baseid for monster '%s' (%d)", szMonsterName);
				}

				if (nId == nCounter)
				{
					bFoundInChain = true;
					gpDataTables.pMonStatsTxt[nCounter].nChainId = nChainId;
				}

				nNextInClass = gpDataTables.pMonStatsTxt[nId].nNextInClass;

				++nChainId;
				if (nId == nNextInClass)
				{
					if (gpDataTables.bCompileTxt)
					{
						szMonsterName = FOG_10255(gpDataTables.pMonStatsLinker, gpDataTables.pMonStatsTxt[nId].nId, 0);
						FOG_WriteToLogFile("BaseId/NextInClass chain -- monster '%s' (%d) pointing to itself", szMonsterName);
					}
					break;
				}

				nId = nNextInClass;

				if (nChainId > 255)
				{
					if (gpDataTables.bCompileTxt)
					{
						szMonsterName = FOG_10255(gpDataTables.pMonStatsLinker, gpDataTables.pMonStatsTxt[nNextInClass].nId, 0);
						FOG_WriteToLogFile("BaseId/NextInClass chain exceeded 255 entries at monster '%s' (%d)", szMonsterName);
					}
					break;
				}
			}
			while (nNextInClass >= 0);

			if (!bFoundInChain && gpDataTables.bCompileTxt)
			{
				szMonsterName = FOG_10255(gpDataTables.pMonStatsLinker, gpDataTables.pMonStatsTxt[nCounter].nId, 0);
				FOG_WriteToLogFile("BaseId/NextInClass monster '%s' (%d) not found in chain", szMonsterName);
			}
			gpDataTables.pMonStatsTxt[nCounter].nMaxChainId = nChainId;

			++nCounter;
		}
		while (nCounter < gpDataTables.nMonStatsTxtRecordCount);
	}


	nCounter = 0;
	while (nCounter < gpDataTables.nMonStatsTxtRecordCount)
	{
		nBaseId = gpDataTables.pMonStatsTxt[nCounter].nBaseId;
		if (nBaseId < 0 || nBaseId >= gpDataTables.nMonStatsTxtRecordCount)
		{
			gpDataTables.pMonStatsTxt[nCounter].nBaseId = nCounter;
			nBaseId = nCounter;
		}

		nVelocity = *((DWORD*)DATATBLS_GetAnimDataRecord(0, nBaseId, MONMODE_WALK, 1, 0) + 3);
		if (nBaseId != nCounter)
		{
			if (gpDataTables.pMonStatsTxt[nBaseId].nVelocity > 0)
			{
				nVelocity = nVelocity * gpDataTables.pMonStatsTxt[nCounter].nVelocity / (unsigned int)gpDataTables.pMonStatsTxt[nBaseId].nVelocity;
			}
		}

		if (nVelocity <= 0)
		{
			gpDataTables.pMonStatsTxt[nCounter].unk0x36 = 0;
		}
		else if (nVelocity >= 32767)
		{
			gpDataTables.pMonStatsTxt[nCounter].unk0x36 = 32767;
		}
		else
		{
			gpDataTables.pMonStatsTxt[nCounter].unk0x36 = nVelocity;
		}

		if (nCounter < 410)//Before Expansion
		{
			v21 = gpDataTables.pMonStatsTxt[nBaseId].unk0x36;
			nRun = ((signed int)v21 - HIDWORD(v21)) >> 1;
		}
		else
		{
			nRun = DATATBLS_GetAnimDataRecord(NULL, nBaseId, MONMODE_RUN, UNIT_MONSTER, NULL)->dwAnimSpeed;
		}

		if (nBaseId != nCounter)
		{
			if (gpDataTables.pMonStatsTxt[nBaseId].nRun > 0)
			{
				nRun = nRun * gpDataTables.pMonStatsTxt[nCounter].nRun / (unsigned int)gpDataTables.pMonStatsTxt[nBaseId].nRun;
			}
		}

		if (nRun <= 0)
		{
			nRun = 0;
		}
		else if (nRun >= 32767)
		{
			nRun = 32767;
		}

		gpDataTables.pMonStatsTxt[nCounter].unk0x38 = nRun;

		++nCounter;
	}
}

//D2Common.0x6FD67BD0 (#11261)
BOOL __stdcall DATATBLS_CalculateMonsterStatsByLevel(int nMonsterId, int nGameType, int nDifficulty, int nLevel, short nFlags, D2MonStatsInitStrc* pMonStatsInit)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2MonLvlTxt* pMonLvlTxtRecord = NULL;
	int nOffset = 0;

	pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
	if (pMonStatsTxtRecord)
	{
		if (nLevel > gpDataTables.nMonLvlTxtRecordCount - 1)
		{
			nLevel = gpDataTables.nMonLvlTxtRecordCount - 1;
		}

		if (nLevel >= 0 && nLevel < gpDataTables.nMonLvlTxtRecordCount)
		{
			pMonLvlTxtRecord = &gpDataTables.pMonLvlTxt[nLevel];
			if (pMonLvlTxtRecord)
			{
				if (nDifficulty < 0)
				{
					nDifficulty = 0;
				}
				else if (nDifficulty > 2)
				{
					nDifficulty = 2;
				}

				nOffset = 3 * (nGameType != 0);

				if (nFlags & 1)
				{
					if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO])
					{
						pMonStatsInit->nMinHP = pMonStatsTxtRecord->nMinHP[nDifficulty];
						pMonStatsInit->nMaxHP = pMonStatsTxtRecord->nMaxHP[nDifficulty];
					}
					else
					{
						pMonStatsInit->nMinHP = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwHP[nDifficulty + nOffset], pMonStatsTxtRecord->nMinHP[nDifficulty], 100);
						pMonStatsInit->nMaxHP = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwHP[nDifficulty + nOffset], pMonStatsTxtRecord->nMaxHP[nDifficulty], 100);
					}
				}

				if (nFlags & 2)
				{
					if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO])
					{
						pMonStatsInit->nAC = pMonStatsTxtRecord->nAC[nDifficulty];
					}
					else
					{
						pMonStatsInit->nAC = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwAC[nDifficulty + nOffset], pMonStatsTxtRecord->nAC[nDifficulty], 100);
					}
				}

				if (nFlags & 4)
				{
					if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO])
					{
						pMonStatsInit->nExp = pMonStatsTxtRecord->nExp[nDifficulty];
					}
					else
					{
						pMonStatsInit->nExp = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwXP[nDifficulty + nOffset], pMonStatsTxtRecord->nExp[nDifficulty], 100);
					}
				}

				if (nFlags & 8)
				{
					if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO])
					{
						pMonStatsInit->nTH = pMonStatsTxtRecord->nA1TH[nDifficulty];
						pMonStatsInit->nA1MinD = pMonStatsTxtRecord->nA1MinD[nDifficulty];
						pMonStatsInit->nA1MaxD = pMonStatsTxtRecord->nA1MaxD[nDifficulty];
					}
					else
					{
						pMonStatsInit->nTH = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwTH[nDifficulty + nOffset], pMonStatsTxtRecord->nA1TH[nDifficulty], 100);
						pMonStatsInit->nA1MinD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nA1MinD[nDifficulty], 100);
						pMonStatsInit->nA1MaxD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nA1MaxD[nDifficulty], 100);
					}
				}

				if (nFlags & 0x10)
				{
					if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO])
					{
						pMonStatsInit->nTH = pMonStatsTxtRecord->nA2TH[nDifficulty];
						pMonStatsInit->nA2MinD = pMonStatsTxtRecord->nA2MinD[nDifficulty];
						pMonStatsInit->nA2MaxD = pMonStatsTxtRecord->nA2MaxD[nDifficulty];
					}
					else
					{
						pMonStatsInit->nTH = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwTH[nDifficulty + nOffset], pMonStatsTxtRecord->nA2TH[nDifficulty], 100);
						pMonStatsInit->nA2MinD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nA2MinD[nDifficulty], 100);
						pMonStatsInit->nA2MaxD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nA2MaxD[nDifficulty], 100);
					}
				}

				if (nFlags & 0x20)
				{
					if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO])
					{
						pMonStatsInit->nTH = pMonStatsTxtRecord->nS1TH[nDifficulty];
						pMonStatsInit->nS1MinD = pMonStatsTxtRecord->nS1MinD[nDifficulty];
						pMonStatsInit->nS1MaxD = pMonStatsTxtRecord->nS1MaxD[nDifficulty];
					}
					else
					{
						pMonStatsInit->nTH = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwTH[nDifficulty + nOffset], pMonStatsTxtRecord->nS1TH[nDifficulty], 100);
						pMonStatsInit->nS1MinD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nS1MinD[nDifficulty], 100);
						pMonStatsInit->nS1MaxD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nS1MaxD[nDifficulty], 100);
					}
				}

				if (nFlags & 0x40)
				{
					if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO])
					{
						pMonStatsInit->nS1MinD = pMonStatsTxtRecord->nElMinD[0][nDifficulty];
						pMonStatsInit->nS1MaxD = pMonStatsTxtRecord->nElMaxD[0][nDifficulty];
					}
					else
					{
						pMonStatsInit->nElMinD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nElMinD[0][nDifficulty], 100);
						pMonStatsInit->nElMaxD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nElMaxD[0][nDifficulty], 100);
					}

					pMonStatsInit->nElDur = pMonStatsTxtRecord->nElDur[0][nDifficulty];
				}

				if (nFlags & 0x80)
				{
					if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO])
					{
						pMonStatsInit->nS1MinD = pMonStatsTxtRecord->nElMinD[1][nDifficulty];
						pMonStatsInit->nS1MaxD = pMonStatsTxtRecord->nElMaxD[1][nDifficulty];
					}
					else
					{
						pMonStatsInit->nElMinD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nElMinD[1][nDifficulty], 100);
						pMonStatsInit->nElMaxD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nElMaxD[1][nDifficulty], 100);
					}

					pMonStatsInit->nElDur = pMonStatsTxtRecord->nElDur[1][nDifficulty];
				}

				if (nFlags & 0x100)
				{
					if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO])
					{
						pMonStatsInit->nS1MinD = pMonStatsTxtRecord->nElMinD[2][nDifficulty];
						pMonStatsInit->nS1MaxD = pMonStatsTxtRecord->nElMaxD[2][nDifficulty];
					}
					else
					{
						pMonStatsInit->nElMinD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nElMinD[2][nDifficulty], 100);
						pMonStatsInit->nElMaxD = DATATBLS_CalculatePercentage(pMonLvlTxtRecord->dwDM[nDifficulty + nOffset], pMonStatsTxtRecord->nElMaxD[2][nDifficulty], 100);
					}

					pMonStatsInit->nElDur = pMonStatsTxtRecord->nElDur[2][nDifficulty];
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD689B0 (#10642)
void __stdcall DATATBLS_SetVelocityInMonStatsTxtRecord(int nMonsterId, short nVelocity)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;

	D2_ASSERT(gpDataTables.pMonStatsTxt);

	pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
	pMonStatsTxtRecord->nVelocity = nVelocity;
}

//D2Common.0x6FD68A00
void __stdcall DATATBLS_ResetGlobalDefaultUnicodeString()
{
	gpDataTables.wszDefault = 0;
}

//D2Common.0x6FD68A10 (#10651)
wchar_t* __fastcall DATATBLS_RollRandomUniqueTitleString(D2UnitStrc* pUnit)
{
	wchar_t* pString = NULL;
	int nRand = 0;

	if (pUnit && gpDataTables.nUniqueTitleTxtRecordCount > 0)
	{
		if ((gpDataTables.nUniqueTitleTxtRecordCount - 1) & gpDataTables.nUniqueTitleTxtRecordCount)
		{
			nRand = (unsigned int)SEED_RollRandomNumber(&pUnit->pSeed) % gpDataTables.nUniqueTitleTxtRecordCount;
		}
		else
		{
			nRand = SEED_RollRandomNumber(&pUnit->pSeed) & (gpDataTables.nUniqueTitleTxtRecordCount - 1);
		}

		if (gpDataTables.pUniqueTitleTxt[nRand].wStringId)
		{
			pString = D2LANG_GetStringFromTblIndex(gpDataTables.pUniqueTitleTxt[nRand].wStringId);
			if (pString)
			{
				return pString;
			}
		}
	}

	return &gpDataTables.wszDefault;
}

//D2Common.0x6FD68A80 (#10652)
wchar_t* __fastcall DATATBLS_RollRandomUniquePrefixString(D2UnitStrc* pUnit)
{
	wchar_t* pString = NULL;
	int nRand = 0;

	if (pUnit && gpDataTables.nUniquePrefixTxtRecordCount > 0)
	{
		if ((gpDataTables.nUniquePrefixTxtRecordCount - 1) & gpDataTables.nUniquePrefixTxtRecordCount)
		{
			nRand = (unsigned int)SEED_RollRandomNumber(&pUnit->pSeed) % gpDataTables.nUniquePrefixTxtRecordCount;
		}
		else
		{
			nRand = SEED_RollRandomNumber(&pUnit->pSeed) & (gpDataTables.nUniquePrefixTxtRecordCount - 1);
		}

		if (gpDataTables.pUniquePrefixTxt[nRand].wStringId)
		{
			pString = D2LANG_GetStringFromTblIndex(gpDataTables.pUniquePrefixTxt[nRand].wStringId);
			if (pString)
			{
				return pString;
			}
		}
	}

	return &gpDataTables.wszDefault;
}

//D2Common.0x6FD68AF0 (#10653)
wchar_t* __fastcall DATATBLS_RollRandomUniqueSuffixString(D2UnitStrc* pUnit)
{
	wchar_t* pString = NULL;
	int nRand = 0;

	if (pUnit && gpDataTables.nUniqueSuffixTxtRecordCount > 0)
	{
		if ((gpDataTables.nUniqueSuffixTxtRecordCount - 1) & gpDataTables.nUniqueSuffixTxtRecordCount)
		{
			nRand = (unsigned int)SEED_RollRandomNumber(&pUnit->pSeed) % gpDataTables.nUniqueSuffixTxtRecordCount;
		}
		else
		{
			nRand = SEED_RollRandomNumber(&pUnit->pSeed) & (gpDataTables.nUniqueSuffixTxtRecordCount - 1);
		}

		if (gpDataTables.pUniqueSuffixTxt[nRand].wStringId)
		{
			pString = D2LANG_GetStringFromTblIndex(gpDataTables.pUniqueSuffixTxt[nRand].wStringId);
			if (pString)
			{
				return pString;
			}
		}
	}

	return &gpDataTables.wszDefault;
}

//D2Common.0x6FD68B60 (#10654)
wchar_t* __fastcall DATATBLS_RollRandomUniqueAppellationString(D2UnitStrc* pUnit)
{
	wchar_t* pString = NULL;
	int nRand = 0;

	if (pUnit && gpDataTables.nUniqueAppellationTxtRecordCount > 0)
	{
		if ((gpDataTables.nUniqueAppellationTxtRecordCount - 1) & gpDataTables.nUniqueAppellationTxtRecordCount)
		{
			nRand = (unsigned int)SEED_RollRandomNumber(&pUnit->pSeed) % gpDataTables.nUniqueAppellationTxtRecordCount;
		}
		else
		{
			nRand = SEED_RollRandomNumber(&pUnit->pSeed) & (gpDataTables.nUniqueAppellationTxtRecordCount - 1);
		}

		if (gpDataTables.pUniqueAppellationTxt[nRand].wStringId)
		{
			pString = D2LANG_GetStringFromTblIndex(gpDataTables.pUniqueAppellationTxt[nRand].wStringId);
			if (pString)
			{
				return pString;
			}
		}
	}

	return &gpDataTables.wszDefault;
}

//D2Common.0x6FD68BD0
void __fastcall DATATBLS_ReallocTCExInfo(D2TCExShortStrc* pTCExShort, int nNewRecordCount)
{
	pTCExShort->pInfo = (D2TCExInfoStrc*)FOG_ReallocServerMemory(NULL, pTCExShort->pInfo, sizeof(D2TCExInfoStrc) * nNewRecordCount, __FILE__, __LINE__, 0);
	memset(&pTCExShort->pInfo[pTCExShort->nTypes], 0x00, sizeof(D2TCExInfoStrc) * (nNewRecordCount - pTCExShort->nTypes));
	pTCExShort->nTypes = nNewRecordCount;
}

//D2Common.0x6FD68C40
void __fastcall DATATBLS_UpdateTreasureClassProbabilities(D2TCExShortStrc* pTCExShort, D2TCExInfoStrc* pTCExInfo, int nProbability, BOOL bExpansion)
{
	int nRecordId = 0;
	int nProbDiff = 0;
	int nProb = 0;

	nRecordId = pTCExInfo - pTCExShort->pInfo;

	if (nRecordId == pTCExShort->nTypes - 1)
	{
		nProb = pTCExShort->nProb;
	}
	else
	{
		nProb = pTCExShort->pInfo[nRecordId + 1].nProb;
	}

	nProbDiff = nProb - pTCExInfo->nProb;

	if (nProbDiff != nProbability)
	{
		pTCExShort->nProb += (nProbability - nProbDiff);

		for (int i = nRecordId + 1; i < pTCExShort->nTypes; ++i)
		{
			pTCExShort->pInfo[i].nProb += (nProbability - nProbDiff);
		}
	}

	if (!bExpansion)
	{
		if (nRecordId == pTCExShort->nTypes - 1)
		{
			nProb = pTCExShort->nClassic;
		}
		else
		{
			nProb = pTCExShort->pInfo[nRecordId + 1].nClassic;
		}

		nProbDiff = nProb - pTCExInfo->nClassic;

		if (nProbDiff != nProbability)
		{
			pTCExShort->nClassic += (nProbability - nProbDiff);
	
			for (int i = nRecordId + 1; i < pTCExShort->nTypes; ++i)
			{
				pTCExShort->pInfo[i].nClassic += (nProbability - nProbDiff);
			}
		}
	}
}

//D2Common.0x6FD68D40
int __fastcall DATATBLS_CalculatePercentage(signed int nValue1, signed int nValue2, signed int nDivisor)
{
	if (nDivisor)
	{
		if (nValue1 <= 1048576)
		{
			if (nValue2 <= 65536)
			{
				return nValue2 * nValue1 / nDivisor;
			}

			if (nDivisor <= nValue2 >> 4)
			{
				return nValue1 * nValue2 / nDivisor;
			}
		}
		else
		{
			if (nDivisor <= nValue1 >> 4)
			{
				return nValue2 * nValue1 / nDivisor;
			}
		}

		return nValue2 * (signed long long int)nValue1 / nDivisor;
	}

	return 0;
}

//D2Common.0x6FD68DC0 (#10658)
D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromName(char* szText)
{
	int nId = FOG_GetRowFromTxt(gpDataTables.pTreasureClassExLinker, szText, 0);
	if (nId >= 0)
	{
		return &gpDataTables.pTreasureClassEx[nId];
	}

	return NULL;
}

//D2Common.0x6FD68DF0 (#10659)
D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromIdAndLevel(WORD wTCId, int nLvl)
{
	D2TCExShortStrc* pTCExRecord = NULL;
	D2TCExShortStrc* pNext = NULL;

	if (wTCId && wTCId < gpDataTables.nTreasureClassEx)
	{
		pTCExRecord = &gpDataTables.pTreasureClassEx[wTCId];
		if (nLvl > 0 && pTCExRecord->nGroup)
		{
			pNext = pTCExRecord + 1;

			while (pNext->nGroup == pTCExRecord->nGroup)
			{
				if (pNext->nLevel > nLvl)
				{
					break;
				}

				pTCExRecord = pNext;
				++pNext;
			}
		}

		return pTCExRecord;
	}
	
	return NULL;
}

//D2Common.0x6FD68E50 (#10660)
D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromActAndDifficulty(int nDifficulty, int nAct, int nIndex)
{
	if (nDifficulty > 0)
	{
		if (nDifficulty >= 2)
		{
			nDifficulty = 2;
		}
	}
	else
	{
		nDifficulty = 0;
	}

	if (nAct > 0)
	{
		if (nAct >= 4)
		{
			nAct = 4;
		}
	}
	else
	{
		nAct = 0;
	}

	if (nIndex > 0)
	{
		if (nIndex >= 2)
		{
			nIndex = 2;
		}
		return gpDataTables.pChestTreasureClasses[nIndex + 3 * (nAct + 5 * nDifficulty)];
	}
	else
	{
		return gpDataTables.pChestTreasureClasses[3 * (nAct + 5 * nDifficulty)];
	}
}

//Inlined in DATATBLS_LoadTreasureClassExTxt
__forceinline void __fastcall DATATBLS_ProcessAdditionalTreasureClassArguments(D2TCExInfoStrc* pTCExInfo, char* szNext, int nCounter, int nItemCounter)
{
	char* szTmp = NULL;
	char* szArg = NULL;

	while (szNext)
	{
		szArg = strchr(szNext, '=');
		if (!szArg)
		{
			FOG_WriteToLogFile("Couldn't parse treasure class modifier!  (Line:%d  Item:%d)\n", nCounter, nItemCounter);
			break;
		}

		*szArg = 0;
		++szArg;
		if (*szArg == 0)
		{
			FOG_WriteToLogFile("Couldn't parse treasure class modifier!  (Line:%d  Item:%d)\n", nCounter, nItemCounter);
			break;
		}

		szTmp = szNext;
		if (!strncmp(szTmp, "mul", 4))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nTxtRow = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cu", 3))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->unk0x14 = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cs", 3))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->unk0x12 = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cr", 3))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->unk0x10 = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cm", 3))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->unk0x0E = atoi(szArg);
		}
		else if (!strncmp(szTmp, "ce", 3))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->unk0x16 = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cg", 3))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->unk0x18 = atoi(szArg);
		}
		else if (!strncmp(szTmp, "ma", 3))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nTxtRow = atoi(szArg);
		}
		else if (!strncmp(szTmp, "mg", 3))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nTxtRow = atoi(szArg);
		}
		else
		{
			FOG_WriteToLogFile("Couldn't parse treasure class modifier!  (Line:%d  Item:%d)\n", nCounter, nItemCounter);
			break;
		}
	}
}

//D2Common.0x6FD68EC0
void __fastcall DATATBLS_LoadTreasureClassExTxt(void* pMemPool)
{
	D2TreasureClassExTxt* pTreasureClassExTxt = NULL;
	D2TCExShortStrc* pTCExTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2TCExInfoStrc* pTCExInfo = NULL;
	char* szNext = NULL;
	char* szText = NULL;
	char* szTmp = NULL;
	int nRecordCount = 0;
	int nItemId = 0;
	int nTxtRow = 0;
	int nTypes = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "treasure class", TXTFIELD_ASCII, 32, 0, NULL },
		{ "group", TXTFIELD_WORD, 0, 36, NULL },
		{ "level", TXTFIELD_WORD, 0, 38, NULL },
		{ "picks", TXTFIELD_DWORD, 0, 32, NULL },
		{ "unique", TXTFIELD_WORD, 0, 46, NULL },
		{ "set", TXTFIELD_WORD, 0, 44, NULL },
		{ "rare", TXTFIELD_WORD, 0, 42, NULL },
		{ "magic", TXTFIELD_WORD, 0, 40, NULL },
		{ "nodrop", TXTFIELD_DWORD, 0, 52, NULL },
		{ "item1", TXTFIELD_ASCII, 64, 56, NULL },
		{ "prob1", TXTFIELD_DWORD, 0, 696, NULL },
		{ "item2", TXTFIELD_ASCII, 64, 120, NULL },
		{ "prob2", TXTFIELD_DWORD, 0, 700, NULL },
		{ "item3", TXTFIELD_ASCII, 64, 184, NULL },
		{ "prob3", TXTFIELD_DWORD, 0, 704, NULL },
		{ "item4", TXTFIELD_ASCII, 64, 248, NULL },
		{ "prob4", TXTFIELD_DWORD, 0, 708, NULL },
		{ "item5", TXTFIELD_ASCII, 64, 312, NULL },
		{ "prob5", TXTFIELD_DWORD, 0, 712, NULL },
		{ "item6", TXTFIELD_ASCII, 64, 376, NULL },
		{ "prob6", TXTFIELD_DWORD, 0, 716, NULL },
		{ "item7", TXTFIELD_ASCII, 64, 440, NULL },
		{ "prob7", TXTFIELD_DWORD, 0, 720, NULL },
		{ "item8", TXTFIELD_ASCII, 64, 504, NULL },
		{ "prob8", TXTFIELD_DWORD, 0, 724, NULL },
		{ "item9", TXTFIELD_ASCII, 64, 568, NULL },
		{ "prob9", TXTFIELD_DWORD, 0, 728, NULL },
		{ "item10", TXTFIELD_ASCII, 64, 632, NULL },
		{ "prob10", TXTFIELD_DWORD, 0, 732, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	pTreasureClassExTxt = (D2TreasureClassExTxt*)DATATBLS_CompileTxt(pMemPool, "treasureclassex", pTbl, &nRecordCount, sizeof(D2TreasureClassExTxt));

	for (int i = 0; i < nRecordCount; ++i)
	{
		if (!pTreasureClassExTxt[i].szTreasureClass[0])
		{
			break;
		}

		FOG_10216_AddRecordToLinkingTable(gpDataTables.pTreasureClassExLinker, pTreasureClassExTxt[i].szTreasureClass);

		if (!(gpDataTables.nTreasureClassEx % 16))
		{
			gpDataTables.pTreasureClassEx = (D2TCExShortStrc*)FOG_ReallocServerMemory(NULL, gpDataTables.pTreasureClassEx, sizeof(D2TCExShortStrc) * (gpDataTables.nTreasureClassEx + 16), __FILE__, __LINE__, 0);
		}
		memset(&gpDataTables.pTreasureClassEx[gpDataTables.nTreasureClassEx], 0x00, sizeof(gpDataTables.pTreasureClassEx[gpDataTables.nTreasureClassEx]));

		pTCExTxtRecord = &gpDataTables.pTreasureClassEx[gpDataTables.nTreasureClassEx];
		++gpDataTables.nTreasureClassEx;
		if (pTCExTxtRecord)
		{
			if (pTreasureClassExTxt[i].nGroup != 0)
			{
				pTCExTxtRecord->nGroup = gpDataTables.nTreasureClassItemTypes + pTreasureClassExTxt[i].nGroup;
			}

			pTCExTxtRecord->nLevel = pTreasureClassExTxt[i].nLevel;
			if (pTreasureClassExTxt[i].nPicks)
			{
				pTCExTxtRecord->nPicks = pTreasureClassExTxt[i].nPicks;
			}
			else
			{
				pTCExTxtRecord->nPicks = 1;
			}

			pTCExTxtRecord->nNoDrop = pTreasureClassExTxt[i].nNoDrop;

			pTCExTxtRecord->nMagic = pTreasureClassExTxt[i].nMagic;
			pTCExTxtRecord->nRare = pTreasureClassExTxt[i].nRare;
			pTCExTxtRecord->nSet = pTreasureClassExTxt[i].nSet;
			pTCExTxtRecord->nUnique = pTreasureClassExTxt[i].nUnique;
			pTCExTxtRecord->nSuperior = pTreasureClassExTxt[i].nSuperior;
			pTCExTxtRecord->nNormal = pTreasureClassExTxt[i].nNormal;

			for (int j = 0; j < 10; ++j)
			{
				if (!pTreasureClassExTxt[i].szItem[j][0])
				{
					break;
				}

				if (pTreasureClassExTxt[i].nProb[j] <= 0)
				{
					FOG_WriteToLogFile("Found a treasure class item with 0 chance!  (Line:%d  Item:%d)\n", i, j);
				}
				else
				{
					szText = pTreasureClassExTxt[i].szItem[j];
					if (szText[0] == '"')
					{
						++szText;
					}

					szTmp = szText;
					if (szTmp[0])
					{
						do
						{
							if (szTmp[0] == '"')
							{
								szTmp[0] = 0;
							}
							++szTmp;
						}
						while (szTmp[0]);
					}

					szNext = strchr(szText, ',');
					if (szNext)
					{
						szNext[0] = 0;
						++szNext;
					}

					if (strlen(szText) <= 4 && (pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(DATATBLS_StringToCode(szText), &nItemId)) != NULL)
					{
						nTypes = pTCExTxtRecord->nTypes;
						DATATBLS_ReallocTCExInfo(pTCExTxtRecord, nTypes + 1);

						pTCExInfo = &pTCExTxtRecord->pInfo[nTypes];

						pTCExInfo->nClassic = pTCExTxtRecord->nClassic;
						pTCExInfo->nProb = pTCExTxtRecord->nProb;
						pTCExInfo->nItemId = nItemId;
						if (pItemsTxtRecord->wVersion >= 100)
						{
							pTCExInfo->nFlags |= 0x10;
						}

						DATATBLS_UpdateTreasureClassProbabilities(pTCExTxtRecord, pTCExInfo, pTreasureClassExTxt[i].nProb[j], pItemsTxtRecord->wVersion >= 100);
						DATATBLS_ProcessAdditionalTreasureClassArguments(pTCExInfo, szNext, i, j);
					}
					else
					{
						nTxtRow = FOG_GetRowFromTxt(gpDataTables.pTreasureClassExLinker, szText, 0);
						if (nTxtRow > 0)
						{
							nTypes = pTCExTxtRecord->nTypes;
							DATATBLS_ReallocTCExInfo(pTCExTxtRecord, nTypes + 1);

							pTCExInfo = &pTCExTxtRecord->pInfo[nTypes];

							pTCExInfo->nClassic = pTCExTxtRecord->nClassic;
							pTCExInfo->nProb = pTCExTxtRecord->nProb;
							pTCExInfo->nItemId = nTxtRow;
							pTCExInfo->nFlags |= 4;
							if (!gpDataTables.pTreasureClassEx[nTxtRow].nClassic)
							{
								pTCExInfo->nFlags |= 0x10;
							}

							DATATBLS_UpdateTreasureClassProbabilities(pTCExTxtRecord, pTCExInfo, pTreasureClassExTxt[i].nProb[j], gpDataTables.pTreasureClassEx[nTxtRow].nClassic == 0);
							DATATBLS_ProcessAdditionalTreasureClassArguments(pTCExInfo, szNext, i, j);
						}
						else if (gpDataTables.pUniqueItemsLinker && (nTxtRow = FOG_GetRowFromTxt(gpDataTables.pUniqueItemsLinker, szText, 0)) > 0)
						{
							nTypes = pTCExTxtRecord->nTypes;
							DATATBLS_ReallocTCExInfo(pTCExTxtRecord, nTypes + 1);

							pTCExInfo = &pTCExTxtRecord->pInfo[nTypes];

							pTCExInfo->nClassic = pTCExTxtRecord->nClassic;
							pTCExInfo->nProb = pTCExTxtRecord->nProb;
							DATATBLS_GetItemRecordFromItemCode(gpDataTables.pUniqueItemsTxt[nTxtRow].dwBaseItemCode, &nItemId);
							pTCExInfo->nFlags |= 0x11;
							pTCExInfo->nItemId = nItemId;
							pTCExInfo->nTxtRow = nTxtRow;

							DATATBLS_UpdateTreasureClassProbabilities(pTCExTxtRecord, pTCExInfo, pTreasureClassExTxt[i].nProb[j], TRUE);
							DATATBLS_ProcessAdditionalTreasureClassArguments(pTCExInfo, szNext, i, j);
						}
						else if (gpDataTables.pSetItemsLinker && (nTxtRow = FOG_GetRowFromTxt(gpDataTables.pSetItemsLinker, szText, 0)) >= 0)
						{
							nTypes = pTCExTxtRecord->nTypes;
							DATATBLS_ReallocTCExInfo(pTCExTxtRecord, nTypes + 1);

							pTCExInfo = &pTCExTxtRecord->pInfo[nTypes];

							pTCExInfo->nClassic = pTCExTxtRecord->nClassic;
							pTCExInfo->nProb = pTCExTxtRecord->nProb;
							DATATBLS_GetItemRecordFromItemCode(gpDataTables.pSetItemsTxt[nTxtRow].szItemCode, &nItemId);
							pTCExInfo->nFlags |= 0x12;
							pTCExInfo->nItemId = nItemId;
							pTCExInfo->nTxtRow = nTxtRow;

							DATATBLS_UpdateTreasureClassProbabilities(pTCExTxtRecord, pTCExInfo, pTreasureClassExTxt[i].nProb[j], TRUE);
							DATATBLS_ProcessAdditionalTreasureClassArguments(pTCExInfo, szNext, i, j);
						}
						else
						{
							FOG_WriteToLogFile("Couldn't parse treasure class item!  (Line:%d  Item:%d)\n", i, j);
						}
					}
				}
			}
		}
	}

	DATATBLS_UnloadBin(pTreasureClassExTxt);
}

//D2Common.0x6FD69B70 (#10656)
int __stdcall DATATBLS_ShouldNeverCallInExpansion()
{
	FOG_10025("should never call this function in expansion", __FILE__, __LINE__);
	return 0;
}

//D2Common.0x6FD69B90
void __fastcall DATATBLS_LoadMonItemPercentTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "HeartPercent", TXTFIELD_BYTE, 0, 0, NULL },
		{ "BodyPartPercent", TXTFIELD_BYTE, 0, 1, NULL },
		{ "TreasureClassPercent", TXTFIELD_BYTE, 0, 2, NULL },
		{ "ComponentPercent", TXTFIELD_BYTE, 0, 3, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonItemPercentDataTables.pMonItemPercentTxt = (D2MonItemPercentTxt*)DATATBLS_CompileTxt(pMemPool, "monitempercent", pTbl, &gpDataTables.pMonItemPercentDataTables.nMonItemPercentTxtRecordCount, sizeof(D2MonItemPercentTxt));
}

//D2Common.0x6FD69C40
void __fastcall DATATBLS_UnloadMonItemPercentTxt()
{
	DATATBLS_UnloadBin(gpDataTables.pMonItemPercentDataTables.pMonItemPercentTxt);
	gpDataTables.pMonItemPercentDataTables.pMonItemPercentTxt = NULL;
}

//D2Common.0x6FD69C50 (#10662)
D2MonItemPercentDataTbl* __fastcall DATATBLS_GetMonItemPercentDataTables()
{
	return &gpDataTables.pMonItemPercentDataTables;
}

//D2Common.0x6FD69C60 (#10663)
D2MonItemPercentTxt* __stdcall DATATBLS_GetMonItemPercentTxtRecord(int nId)
{
	if (nId >= 0 && nId < gpDataTables.pMonItemPercentDataTables.nMonItemPercentTxtRecordCount)
	{
		D2_ASSERT(gpDataTables.pMonItemPercentDataTables.pMonItemPercentTxt);
		D2_ASSERT(&gpDataTables.pMonItemPercentDataTables.pMonItemPercentTxt[nId]);
		return &gpDataTables.pMonItemPercentDataTables.pMonItemPercentTxt[nId];
	}

	return NULL;
}

//D2Common.0x6FD69CD0
void __fastcall DATATBLS_LoadMonUModTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "uniquemod", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pMonUModLinker },
		{ "version", TXTFIELD_WORD, 0, 4, NULL },
		{ "enabled", TXTFIELD_BYTE, 0, 6, NULL },
		{ "xfer", TXTFIELD_BYTE, 0, 7, NULL },
		{ "champion", TXTFIELD_BYTE, 0, 8, NULL },
		{ "fpick", TXTFIELD_BYTE, 0, 9, NULL },
		{ "exclude1", TXTFIELD_NAMETOWORD, 0, 10, &gpDataTables.pMonTypeLinker },
		{ "exclude2", TXTFIELD_NAMETOWORD, 0, 12, &gpDataTables.pMonTypeLinker },
		{ "cpick", TXTFIELD_WORD, 0, 14, NULL },
		{ "cpick (N)", TXTFIELD_WORD, 0, 16, NULL },
		{ "cpick (H)", TXTFIELD_WORD, 0, 18, NULL },
		{ "upick", TXTFIELD_WORD, 0, 20, NULL },
		{ "upick (N)", TXTFIELD_WORD, 0, 22, NULL },
		{ "upick (H)", TXTFIELD_WORD, 0, 24, NULL },
		{ "constants", TXTFIELD_DWORD, 0, 28, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonUModLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pMonUModTxt = (D2MonUModTxt*)DATATBLS_CompileTxt(pMemPool, "monumod", pTbl, &gpDataTables.nMonUModTxtRecordCount, sizeof(D2MonUModTxt));

	if (gpDataTables.nMonUModTxtRecordCount > 256)
	{
		FOG_WriteToLogFile("monumod.txt exceeded %d entries", 256);
		gpDataTables.nMonUModTxtRecordCount = 256;
	}
}

//D2Common.0x6FD69FA0
void __fastcall DATATBLS_LoadSuperUniquesTxt(void* pMemPool)
{
	int nId = 0;
	D2BinFieldStrc pTbl[] =
	{
		{ "Superunique", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pSuperUniquesLinker },
		{ "Name", TXTFIELD_KEYTOWORD, 0, TXTFIELD_DWORD, DATATBLS_GetStringIdFromReferenceString },
		{ "Class", TXTFIELD_NAMETODWORD, 0, 4, &gpDataTables.pMonStatsLinker },
		{ "hcIdx", TXTFIELD_DWORD, 0, 8, NULL },
		{ "MonSound", TXTFIELD_NAMETODWORD, 0, 24, &gpDataTables.pMonSoundsLinker },
		{ "Mod1", TXTFIELD_DWORD, 0, 12, NULL },
		{ "Mod2", TXTFIELD_DWORD, 0, 16, NULL },
		{ "Mod3", TXTFIELD_DWORD, 0, 20, NULL },
		{ "MinGrp", TXTFIELD_DWORD, 0, 28, NULL },
		{ "MaxGrp", TXTFIELD_DWORD, 0, 32, NULL },
		{ "AutoPos", TXTFIELD_BYTE, 0, 36, NULL },
		{ "EClass", TXTFIELD_BYTE, 0, 37, NULL },
		{ "Stacks", TXTFIELD_BYTE, 0, 38, NULL },
		{ "Replaceable", TXTFIELD_BYTE, 0, 39, NULL },
		{ "Utrans", TXTFIELD_BYTE, 0, 40, NULL },
		{ "Utrans(N)", TXTFIELD_BYTE, 0, 41, NULL },
		{ "Utrans(H)", TXTFIELD_BYTE, 0, 42, NULL },
		{ "TC", TXTFIELD_NAMETOWORD, 0, 44, &gpDataTables.pTreasureClassExLinker },
		{ "TC(N)", TXTFIELD_NAMETOWORD, 0, 46, &gpDataTables.pTreasureClassExLinker },
		{ "TC(H)", TXTFIELD_NAMETOWORD, 0, 48, &gpDataTables.pTreasureClassExLinker },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pSuperUniquesLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pSuperUniquesTxt = (D2SuperUniquesTxt*)DATATBLS_CompileTxt(pMemPool, "superuniques", pTbl, &gpDataTables.nSuperUniquesTxtRecordCount, sizeof(D2SuperUniquesTxt));

	if (gpDataTables.nSuperUniquesTxtRecordCount >= 512)
	{
		if (gpDataTables.bCompileTxt)
		{
			FOG_WriteToLogFile("Cut off superuniques at %d entries", 512);
		}
		gpDataTables.nSuperUniquesTxtRecordCount = 512;
	}

	memset(gpDataTables.nSuperUniqueIds, -1, sizeof(gpDataTables.nSuperUniqueIds));

	for (int i = 0; i < gpDataTables.nSuperUniquesTxtRecordCount; ++i)
	{
		nId = gpDataTables.pSuperUniquesTxt[i].dwHcIdx;
		if (nId < 0 || nId >= ARRAY_SIZE(gpDataTables.nSuperUniqueIds))
		{
			if (gpDataTables.bCompileTxt)
			{
				FOG_WriteToLogFile("Invalid hcIdx (%d) -- value must be between 0 and %d.", nId, 65);
			}
		}
		else
		{
			if (gpDataTables.nSuperUniqueIds[nId] == -1)
			{
				gpDataTables.nSuperUniqueIds[nId] = i;
			}
			else
			{
				if (gpDataTables.bCompileTxt)
				{
					FOG_WriteToLogFile("Duplicate hcIdx (%d) -- index must be unique.", nId);
				}
			}
		}
	}

	for (int i = 0; i < ARRAY_SIZE(gpDataTables.nSuperUniqueIds); ++i)
	{
		if (gpDataTables.nSuperUniqueIds[i] == -1 && gpDataTables.bCompileTxt)
		{
			FOG_WriteToLogFile("Missing hcIdx (%d) -- index incomplete", i);
		}

#define SUPERUNIQUE_NONE -1
		D2_ASSERT(gpDataTables.nSuperUniqueIds[i] != SUPERUNIQUE_NONE);
	}
}

//D2Common.0x6FD6A440 (#10668)
D2SuperUniquesTxt* __stdcall DATATBLS_GetSuperUniquesTxtRecord(int nSuperUniqueId)
{
	if (nSuperUniqueId >= 0 && nSuperUniqueId < gpDataTables.nSuperUniquesTxtRecordCount)
	{
		return &gpDataTables.pSuperUniquesTxt[nSuperUniqueId];
	}

	return NULL;
}

//D2Common.0x6FD6A470 (#11257)
int __fastcall DATATBLS_GetSuperUniquesTxtRecordCount()
{
	return gpDataTables.nSuperUniquesTxtRecordCount;
}

//D2Common.0x6FD6A480
void __fastcall DATATBLS_LoadHirelingTxt(void* pMemPool)
{
	wchar_t* pUnicode = NULL;
	int nId = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "version", TXTFIELD_WORD, 0, 0, NULL },
		{ "id", TXTFIELD_DWORD, 0, TXTFIELD_BYTE, NULL },
		{ "class", TXTFIELD_DWORD, 0, 8, NULL },
		{ "act", TXTFIELD_DWORD, 0, 12, NULL },
		{ "difficulty", TXTFIELD_DWORD, 0, 16, NULL },
		{ "seller", TXTFIELD_DWORD, 0, 20, NULL },
		{ "gold", TXTFIELD_DWORD, 0, 24, NULL },
		{ "level", TXTFIELD_DWORD, 0, 28, NULL },
		{ "namefirst", TXTFIELD_ASCII, 32, 211, NULL },
		{ "namelast", TXTFIELD_ASCII, 32, 243, NULL },
		{ "exp/lvl", TXTFIELD_DWORD, 0, 32, NULL },
		{ "hp", TXTFIELD_DWORD, 0, 36, NULL },
		{ "hp/lvl", TXTFIELD_DWORD, 0, 40, NULL },
		{ "defense", TXTFIELD_DWORD, 0, 44, NULL },
		{ "def/lvl", TXTFIELD_DWORD, 0, 48, NULL },
		{ "str", TXTFIELD_DWORD, 0, 52, NULL },
		{ "str/lvl", TXTFIELD_DWORD, 0, 56, NULL },
		{ "dex", TXTFIELD_DWORD, 0, 60, NULL },
		{ "dex/lvl", TXTFIELD_DWORD, 0, 64, NULL },
		{ "ar", TXTFIELD_DWORD, 0, 68, NULL },
		{ "ar/lvl", TXTFIELD_DWORD, 0, 72, NULL },
		{ "share", TXTFIELD_DWORD, 0, 76, NULL },
		{ "dmg-min", TXTFIELD_DWORD, 0, 80, NULL },
		{ "dmg-max", TXTFIELD_DWORD, 0, 84, NULL },
		{ "dmg/lvl", TXTFIELD_DWORD, 0, 88, NULL },
		{ "resist", TXTFIELD_DWORD, 0, 92, NULL },
		{ "resist/lvl", TXTFIELD_DWORD, 0, 96, NULL },
		{ "hiredesc", TXTFIELD_CODETOBYTE, 0, 210, &gpDataTables.pHireDescLinker },
		{ "defaultchance", TXTFIELD_DWORD, 0, 100, NULL },
		{ "skill1", TXTFIELD_NAMETODWORD, 0, 120, &gpDataTables.pSkillsLinker },
		{ "mode1", TXTFIELD_BYTE, 0, 192, NULL },
		{ "chance1", TXTFIELD_DWORD, 0, 144, NULL },
		{ "chanceperlvl1", TXTFIELD_DWORD, 0, 168, NULL },
		{ "level1", TXTFIELD_BYTE, 0, 198, NULL },
		{ "lvlperlvl1", TXTFIELD_BYTE, 0, 204, NULL },
		{ "skill2", TXTFIELD_NAMETODWORD, 0, 124, &gpDataTables.pSkillsLinker },
		{ "mode2", TXTFIELD_BYTE, 0, 193, NULL },
		{ "chance2", TXTFIELD_DWORD, 0, 148, NULL },
		{ "chanceperlvl2", TXTFIELD_DWORD, 0, 172, NULL },
		{ "level2", TXTFIELD_BYTE, 0, 199, NULL },
		{ "lvlperlvl2", TXTFIELD_BYTE, 0, 205, NULL },
		{ "skill3", TXTFIELD_NAMETODWORD, 0, 128, &gpDataTables.pSkillsLinker },
		{ "mode3", TXTFIELD_BYTE, 0, 194, NULL },
		{ "chance3", TXTFIELD_DWORD, 0, 152, NULL },
		{ "chanceperlvl3", TXTFIELD_DWORD, 0, 176, NULL },
		{ "level3", TXTFIELD_BYTE, 0, 200, NULL },
		{ "lvlperlvl3", TXTFIELD_BYTE, 0, 206, NULL },
		{ "skill4", TXTFIELD_NAMETODWORD, 0, 132, &gpDataTables.pSkillsLinker },
		{ "mode4", TXTFIELD_BYTE, 0, 195, NULL },
		{ "chance4", TXTFIELD_DWORD, 0, 156, NULL },
		{ "chanceperlvl4", TXTFIELD_DWORD, 0, 180, NULL },
		{ "level4", TXTFIELD_BYTE, 0, 201, NULL },
		{ "lvlperlvl4", TXTFIELD_BYTE, 0, 207, NULL },
		{ "skill5", TXTFIELD_NAMETODWORD, 0, 136, &gpDataTables.pSkillsLinker },
		{ "mode5", TXTFIELD_BYTE, 0, 196, NULL },
		{ "chance5", TXTFIELD_DWORD, 0, 160, NULL },
		{ "chanceperlvl5", TXTFIELD_DWORD, 0, 184, NULL },
		{ "level5", TXTFIELD_BYTE, 0, 202, NULL },
		{ "lvlperlvl5", TXTFIELD_BYTE, 0, 208, NULL },
		{ "skill6", TXTFIELD_NAMETODWORD, 0, 140, &gpDataTables.pSkillsLinker },
		{ "mode6", TXTFIELD_BYTE, 0, 197, NULL },
		{ "chance6", TXTFIELD_DWORD, 0, 164, NULL },
		{ "chanceperlvl6", TXTFIELD_DWORD, 0, 188, NULL },
		{ "level6", TXTFIELD_BYTE, 0, 203, NULL },
		{ "lvlperlvl6", TXTFIELD_BYTE, 0, 209, NULL },
		{ "head", TXTFIELD_DWORD, 0, 104, NULL },
		{ "torso", TXTFIELD_DWORD, 0, 108, NULL },
		{ "weapon", TXTFIELD_DWORD, 0, 112, NULL },
		{ "shield", TXTFIELD_DWORD, 0, 116, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pHirelingTxt = (D2HirelingTxt*)DATATBLS_CompileTxt(pMemPool, "hireling", pTbl, &gpDataTables.nHirelingTxtRecordCount, sizeof(D2HirelingTxt));

	for (int i = 0; i < 256; ++i)
	{
		gpDataTables.nClassicHirelingStartRecordIds[i] = -1;
		gpDataTables.nExpansionHirelingStartRecordIds[i] = -1;
	}

	for (int i = 0; i < gpDataTables.nHirelingTxtRecordCount; ++i)
	{
		gpDataTables.pHirelingTxt[i].wNameFirst = D2LANG_GetTblIndex(gpDataTables.pHirelingTxt[i].szNameFirst, &pUnicode);
		gpDataTables.pHirelingTxt[i].wNameLast = D2LANG_GetTblIndex(gpDataTables.pHirelingTxt[i].szNameLast, &pUnicode);

		if (FOG_IsExpansion())
		{
			D2_ASSERT(gpDataTables.pHirelingTxt[i].wNameFirst > 0);
			D2_ASSERT(gpDataTables.pHirelingTxt[i].wNameLast > gpDataTables.pHirelingTxt[i].wNameFirst);

#define MAX_HIRELING_ID 256
			D2_ASSERT(gpDataTables.pHirelingTxt[i].nId < MAX_HIRELING_ID);
		}

		if (gpDataTables.pHirelingTxt[i].nId < 256)
		{
			nId = gpDataTables.pHirelingTxt[i].nId + ((gpDataTables.pHirelingTxt[i].wVersion >= 100) << 8);
			if (gpDataTables.nClassicHirelingStartRecordIds[nId] < 0)
			{
				gpDataTables.nClassicHirelingStartRecordIds[nId] = i;
			}
		}
	}
}

//D2Common.0x6FD6B1A0
D2ItemTypesTxt* __fastcall DATATBLS_GetItemTypesTxtRecord(int nItemType)
{
	if (nItemType >= 0 && nItemType < gpDataTables.nItemTypesTxtRecordCount)
	{
		return &gpDataTables.pItemTypesTxt[nItemType];
	}

	return NULL;
}

//D2Common.0x6FD6B1D0 (#10583)
D2HirelingTxt* __stdcall DATATBLS_GetHirelingTxtRecordFromIdAndLevel(BOOL bExpansion, int nId, int nLevel)
{
	D2HirelingTxt* pHirelingTxtRecord = NULL;
	D2HirelingTxt* pLastRecord = NULL;
	int nStartRecordId = 0;
	int nVersion = 0;

	//bool v10; // zf@7
	//bool v11; // sf@7
	//unsigned __int8 v12; // of@7

	if (nId >= 256)
	{
		return NULL;
	}

	nVersion = bExpansion != 0 ? 100 : 0;

	nStartRecordId = gpDataTables.nClassicHirelingStartRecordIds[nId + ((bExpansion != FALSE) << 8)];

	if (nStartRecordId >= 0 && nStartRecordId < gpDataTables.nHirelingTxtRecordCount)
	{
		pHirelingTxtRecord = &gpDataTables.pHirelingTxt[nStartRecordId];

		while (nStartRecordId < gpDataTables.nHirelingTxtRecordCount)
		{
			if (pHirelingTxtRecord->nId != nId)
			{
				//TODO: Check correctness
				//v12 = __OFSUB__(pHirelingTxtRecord->nId, nId);
				//v10 = pHirelingTxtRecord->nId == nId;
				//v11 = pHirelingTxtRecord->nId < nId;

				if (pHirelingTxtRecord->nId > nId /*!((unsigned __int8)(v11 ^ v12) | v10)*/) //TODO: Or <=
				{
					return pLastRecord;
				}
			}
			else if (pHirelingTxtRecord->wVersion != nVersion)
			{
				//TODO: Check correctness
				//v12 = __OFSUB__(pHirelingTxtRecord->nId, nId);
				//v10 = pHirelingTxtRecord->nId == nId;
				//v11 = pHirelingTxtRecord->nId < nId;

				if (pHirelingTxtRecord->nId > nId /*!((unsigned __int8)(v11 ^ v12) | v10)*/) //TODO: Or <=
				{
					return pLastRecord;
				}
			}
			else if (pLastRecord)
			{
				if (pHirelingTxtRecord->nHirelingLevel > nLevel)
				{
					return pLastRecord;
				}

				pLastRecord = pHirelingTxtRecord;
			}
			else
			{
				pLastRecord = pHirelingTxtRecord;
			}

			++nStartRecordId;
			++pHirelingTxtRecord;
		}

		return pLastRecord;
	}

	return NULL;
}

//D2Common.0x6FD6B270 (#10585)
D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromNameId(BOOL bExpansion, WORD nNameId, D2HirelingTxt* pOldRecord)
{
	D2HirelingTxt* pHirelingTxtRecord = NULL;
	int nRecordId = 0;
	int nVersion = 0;
	int nRand = 0;

	nVersion = bExpansion != 0 ? 100 : 0;

	if (pOldRecord)
	{
		nRecordId = pOldRecord - gpDataTables.pHirelingTxt + 1;
	}

	while (nRecordId < gpDataTables.nHirelingTxtRecordCount)
	{
		pHirelingTxtRecord = &gpDataTables.pHirelingTxt[nRecordId];

		if (nNameId >= pHirelingTxtRecord->wNameFirst && nNameId <= pHirelingTxtRecord->wNameLast && pHirelingTxtRecord->wVersion == nVersion)
		{
			return pHirelingTxtRecord;
		}

		++nRecordId;
	}

	return NULL;
}

//D2Common.0x6FD6B310 (#10586)
D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromClassId(BOOL bExpansion, int nClass, D2HirelingTxt* pOldRecord)
{
	D2HirelingTxt* pHirelingTxtRecord = NULL;
	int nRecordId = 0;
	int nVersion = 0;

	nVersion = bExpansion != 0 ? 100 : 0;

	if (pOldRecord)
	{
		nRecordId = pOldRecord - gpDataTables.pHirelingTxt + 1;
	}

	while (nRecordId < gpDataTables.nHirelingTxtRecordCount)
	{
		pHirelingTxtRecord = &gpDataTables.pHirelingTxt[nRecordId];

		if (pHirelingTxtRecord->dwClass == nClass && pHirelingTxtRecord->wVersion == nVersion)
		{
			return pHirelingTxtRecord;
		}

		++nRecordId;
	}

	return NULL;
}

//D2Common.0x6FD6B3A0 (#10587)
D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty(BOOL bExpansion, int nVendorId, int nDifficulty, D2HirelingTxt* pOldRecord)
{
	D2HirelingTxt* pHirelingTxtRecord = NULL;
	int nRecordId = 0;
	int nVersion = 0;

	nVersion = bExpansion != 0 ? 100 : 0;

	if (pOldRecord)
	{
		nRecordId = pOldRecord - gpDataTables.pHirelingTxt + 1;
	}

	while (nRecordId < gpDataTables.nHirelingTxtRecordCount)
	{
		pHirelingTxtRecord = &gpDataTables.pHirelingTxt[nRecordId];

		if (pHirelingTxtRecord->dwSeller == nVendorId && pHirelingTxtRecord->dwDifficulty == (nDifficulty + 1) && pHirelingTxtRecord->wVersion == nVersion)
		{
			return pHirelingTxtRecord;
		}

		++nRecordId;
	}

	return NULL;
}

//D2Common.0x6FD6B440 (#10584)
D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromActAndDifficulty(BOOL bExpansion, int nAct, int nDifficulty, D2HirelingTxt* pOldRecord)
{
	D2HirelingTxt* pHirelingTxtRecord = NULL;
	int nRecordId = 0;
	int nVersion = 0;
	DWORD nLevel = 0;

	nVersion = bExpansion != 0 ? 100 : 0;

	if (pOldRecord)
	{
		nLevel = pOldRecord->nHirelingLevel;
		nRecordId = pOldRecord - gpDataTables.pHirelingTxt + 1;
	}

	while (nRecordId < gpDataTables.nHirelingTxtRecordCount)
	{
		pHirelingTxtRecord = &gpDataTables.pHirelingTxt[nRecordId];

		if (pHirelingTxtRecord->dwAct == (nAct + 1))
		{
			if (pHirelingTxtRecord->dwDifficulty == (nDifficulty + 1) && pHirelingTxtRecord->wVersion == nVersion && (!nLevel || pHirelingTxtRecord->nHirelingLevel == nLevel))
			{
				return pHirelingTxtRecord;
			}
		}

		++nRecordId;
	}

	return NULL;
}

//D2Common.0x6FD6B4F0
void __fastcall DATATBLS_LoadNpcTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "npc", TXTFIELD_NAMETODWORD, 0, 0, &gpDataTables.pMonStatsLinker },
		{ "sell mult", TXTFIELD_DWORD, 0, 4, NULL },
		{ "buy mult", TXTFIELD_DWORD, 0, 8, NULL },
		{ "rep mult", TXTFIELD_DWORD, 0, 12, NULL },
		{ "questflag A", TXTFIELD_DWORD, 0, 16, NULL },
		{ "questflag B", TXTFIELD_DWORD, 0, 20, NULL },
		{ "questflag C", TXTFIELD_DWORD, 0, 24, NULL },
		{ "questsellmult A", TXTFIELD_DWORD, 0, 28, NULL },
		{ "questsellmult B", TXTFIELD_DWORD, 0, 32, NULL },
		{ "questsellmult C", TXTFIELD_DWORD, 0, 36, NULL },
		{ "questbuymult A", TXTFIELD_DWORD, 0, 40, NULL },
		{ "questbuymult B", TXTFIELD_DWORD, 0, 44, NULL },
		{ "questbuymult C", TXTFIELD_DWORD, 0, 48, NULL },
		{ "questrepmult A", TXTFIELD_DWORD, 0, 52, NULL },
		{ "questrepmult B", TXTFIELD_DWORD, 0, 56, NULL },
		{ "questrepmult C", TXTFIELD_DWORD, 0, 60, NULL },
		{ "max buy", TXTFIELD_DWORD, 0, 64, NULL },
		{ "max buy (N)", TXTFIELD_DWORD, 0, 68, NULL },
		{ "max buy (H)", TXTFIELD_DWORD, 0, 72, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pNpcTxt = (D2NpcTxt*)DATATBLS_CompileTxt(pMemPool, "npc", pTbl, &gpDataTables.nNpcTxtRecordCount, sizeof(D2NpcTxt));
}

//D2Common.0x6FD6B820 (#10588)
D2NpcTxt* __stdcall DATATBLS_GetNpcTxtRecord(DWORD dwNpcId)
{
	for (int i = 0; i < gpDataTables.nNpcTxtRecordCount; ++i)
	{
		if (gpDataTables.pNpcTxt[i].dwNpc == dwNpcId)
		{
			return &gpDataTables.pNpcTxt[i];
		}
	}

	return NULL;
}

//D2Common.0x6FD6B850
void __fastcall DATATBLS_LoadMonSoundsTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Id", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pMonSoundsLinker },
		{ "Attack1", TXTFIELD_NAMETODWORD, 0, 4, &gpDataTables.pSoundsLinker },
		{ "Att1Del", TXTFIELD_DWORD, 0, 8, NULL },
		{ "Att1Prb", TXTFIELD_DWORD, 0, 12, NULL },
		{ "Attack2", TXTFIELD_NAMETODWORD, 0, 28, &gpDataTables.pSoundsLinker },
		{ "Att2Del", TXTFIELD_DWORD, 0, 32, NULL },
		{ "Att2Prb", TXTFIELD_DWORD, 0, 36, NULL },
		{ "Weapon1", TXTFIELD_NAMETODWORD, 0, 16, &gpDataTables.pSoundsLinker },
		{ "Wea1Del", TXTFIELD_DWORD, 0, 20, NULL },
		{ "Wea1Vol", TXTFIELD_DWORD, 0, 24, NULL },
		{ "Weapon2", TXTFIELD_NAMETODWORD, 0, 40, &gpDataTables.pSoundsLinker },
		{ "Wea2Del", TXTFIELD_DWORD, 0, 44, NULL },
		{ "Wea2Vol", TXTFIELD_DWORD, 0, 48, NULL },
		{ "HitSound", TXTFIELD_NAMETODWORD, 0, 52, &gpDataTables.pSoundsLinker },
		{ "HitDelay", TXTFIELD_DWORD, 0, 56, NULL },
		{ "DeathSound", TXTFIELD_NAMETODWORD, 0, 60, &gpDataTables.pSoundsLinker },
		{ "DeaDelay", TXTFIELD_DWORD, 0, 64, NULL },
		{ "Skill1", TXTFIELD_NAMETODWORD, 0, 68, &gpDataTables.pSoundsLinker },
		{ "Skill2", TXTFIELD_NAMETODWORD, 0, 72, &gpDataTables.pSoundsLinker },
		{ "Skill3", TXTFIELD_NAMETODWORD, 0, 76, &gpDataTables.pSoundsLinker },
		{ "Skill4", TXTFIELD_NAMETODWORD, 0, 80, &gpDataTables.pSoundsLinker },
		{ "Footstep", TXTFIELD_NAMETODWORD, 0, 84, &gpDataTables.pSoundsLinker },
		{ "FootstepLayer", TXTFIELD_NAMETODWORD, 0, 88, &gpDataTables.pSoundsLinker },
		{ "FsCnt", TXTFIELD_DWORD, 0, 92, NULL },
		{ "FsOff", TXTFIELD_DWORD, 0, 96, NULL },
		{ "FsPrb", TXTFIELD_DWORD, 0, 100, NULL },
		{ "Neutral", TXTFIELD_NAMETODWORD, 0, 104, &gpDataTables.pSoundsLinker },
		{ "NeuTime", TXTFIELD_DWORD, 0, 108, NULL },
		{ "Init", TXTFIELD_NAMETODWORD, 0, 112, &gpDataTables.pSoundsLinker },
		{ "Taunt", TXTFIELD_NAMETODWORD, 0, 116, &gpDataTables.pSoundsLinker },
		{ "Flee", TXTFIELD_NAMETODWORD, 0, 120, &gpDataTables.pSoundsLinker },
		{ "CvtMo1", TXTFIELD_CODETOBYTE, 0, 124, &gpDataTables.pMonModeLinker },
		{ "CvtMo2", TXTFIELD_CODETOBYTE, 0, 132, &gpDataTables.pMonModeLinker },
		{ "CvtMo3", TXTFIELD_CODETOBYTE, 0, 140, &gpDataTables.pMonModeLinker },
		{ "CvtSk1", TXTFIELD_NAMETODWORD, 0, 128, &gpDataTables.pSkillsLinker },
		{ "CvtSk2", TXTFIELD_NAMETODWORD, 0, 136, &gpDataTables.pSkillsLinker },
		{ "CvtSk3", TXTFIELD_NAMETODWORD, 0, 144, &gpDataTables.pSkillsLinker },
		{ "CvtTgt1", TXTFIELD_CODETOBYTE, 0, 125, &gpDataTables.pMonModeLinker },
		{ "CvtTgt2", TXTFIELD_CODETOBYTE, 0, 133, &gpDataTables.pMonModeLinker },
		{ "CvtTgt3", TXTFIELD_CODETOBYTE, 0, 141, &gpDataTables.pMonModeLinker },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonSoundsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pMonSoundsTxt = (D2MonSoundsTxt*)DATATBLS_CompileTxt(pMemPool, "monsounds", pTbl, &gpDataTables.nMonSoundsTxtRecordCount, sizeof(D2MonSoundsTxt));
}

//D2Common.0x6FD6BF50 (#11252)
D2MonSoundsTxt* __stdcall DATATBLS_GetMonSoundsTxtRecordFromSoundId(int nSoundId)
{
	if (gpDataTables.pMonSoundsTxt && nSoundId >= 0 && nSoundId < gpDataTables.nMonSoundsTxtRecordCount)
	{
		return &gpDataTables.pMonSoundsTxt[nSoundId];
	}

	return NULL;
}

//D2Common.0x6FD6BF80 (#11250)
D2MonSoundsTxt* __stdcall DATATBLS_GetMonSoundsTxtRecordFromMonsterId(int nMonsterId)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nSoundId = 0;

	pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
	if (gpDataTables.pMonSoundsTxt && pMonStatsTxtRecord)
	{
		nSoundId = pMonStatsTxtRecord->wMonSound;
		if (nSoundId >= 0 && nSoundId < gpDataTables.nMonSoundsTxtRecordCount)
		{
			return &gpDataTables.pMonSoundsTxt[nSoundId];
		}
	}

	return NULL;
}

//D2Common.0x6FD6BFE0
void __fastcall DATATBLS_MonStats2CompositLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	char* pSrcCopy = NULL;
	char* pTmp = NULL;
	int nCounter = 0;
	int nSum = 0;
	char szCode[4] = {};

	pMonStats2TxtRecord = (D2MonStats2Txt*)pRecord;
	if (pMonStats2TxtRecord && nOffset >= 0 && nOffset < 16)
	{
		pMonStats2TxtRecord->unk0x15[nOffset] = 0;

		memset(&pMonStats2TxtRecord->unk0x26[nOffset], -1, sizeof(D2UnkMonCompStrc));

		if (pSrc)
		{
			pSrcCopy = pSrc;
			if (*pSrc == '"')
			{
				++pSrcCopy;
			}

			pTmp = pSrcCopy;
			while (*pTmp)
			{
				if (*pTmp == '"')
				{
					*pTmp = 0;
				}
				++pTmp;
			}

			nCounter = 0;
			while (1)
			{
				pTmp = pSrcCopy;
				pSrcCopy = strchr(pSrcCopy, ',');
				if (pSrcCopy)
				{
					*pSrcCopy = 0;
					++pSrcCopy;
				}

				if (!pTmp || !*pTmp)
				{
					break;
				}

				if (strlen(pTmp) > 4)
				{
					break;
				}


				pMonStats2TxtRecord->unk0x26[nOffset].nComposit[nCounter] = FOG_GetLinkIndex(gpDataTables.pCompCodeLinker, DATATBLS_StringToCode(pTmp), 1);

				++nCounter;

				if (!pSrcCopy || nCounter >= 12)
				{
					break;
				}
			}

			pMonStats2TxtRecord->unk0x15[nOffset] = nCounter;

			if (nOffset == 15)
			{
				nSum = 0;
				for (int i = 0; i < 16; ++i)
				{
					if (pMonStats2TxtRecord->unk0x15[i] > 1)
					{
						nSum += pMonStats2TxtRecord->unk0x15[i] - 1;
					}
				}

				if (nSum > 254)
				{
					nSum = 254;
				}
				pMonStats2TxtRecord->unk0x25 = nSum;
			}
		}
	}
}

//D2Common.0x6FD6C150
void __fastcall DATATBLS_LoadMonStats2Txt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Id", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pMonStats2Linker },
		{ "Height", TXTFIELD_BYTE, 0, 11, NULL },
		{ "overlayHeight", TXTFIELD_BYTE, 0, 12, NULL },
		{ "pixHeight", TXTFIELD_BYTE, 0, 13, NULL },
		{ "SizeX", TXTFIELD_BYTE, 0, 8, NULL },
		{ "SizeY", TXTFIELD_BYTE, 0, 9, NULL },
		{ "spawnCol", TXTFIELD_BYTE, 0, 10, NULL },
		{ "MeleeRng", TXTFIELD_BYTE, 0, 14, NULL },
		{ "BaseW", TXTFIELD_RAW, 0, 16, NULL },
		{ "HitClass", TXTFIELD_BYTE, 0, 20, NULL },
		{ "HDv", TXTFIELD_CUSTOMLINK, 0, 0, DATATBLS_MonStats2CompositLinker },
		{ "TRv", TXTFIELD_CUSTOMLINK, 0, 1, DATATBLS_MonStats2CompositLinker },
		{ "LGv", TXTFIELD_CUSTOMLINK, 0, 2, DATATBLS_MonStats2CompositLinker },
		{ "Rav", TXTFIELD_CUSTOMLINK, 0, 3, DATATBLS_MonStats2CompositLinker },
		{ "Lav", TXTFIELD_CUSTOMLINK, 0, 4, DATATBLS_MonStats2CompositLinker },
		{ "RHv", TXTFIELD_CUSTOMLINK, 0, 5, DATATBLS_MonStats2CompositLinker },
		{ "LHv", TXTFIELD_CUSTOMLINK, 0, 6, DATATBLS_MonStats2CompositLinker },
		{ "SHv", TXTFIELD_CUSTOMLINK, 0, 7, DATATBLS_MonStats2CompositLinker },
		{ "S1v", TXTFIELD_CUSTOMLINK, 0, 8, DATATBLS_MonStats2CompositLinker },
		{ "S2v", TXTFIELD_CUSTOMLINK, 0, 9, DATATBLS_MonStats2CompositLinker },
		{ "S3v", TXTFIELD_CUSTOMLINK, 0, 10, DATATBLS_MonStats2CompositLinker },
		{ "S4v", TXTFIELD_CUSTOMLINK, 0, 11, DATATBLS_MonStats2CompositLinker },
		{ "S5v", TXTFIELD_CUSTOMLINK, 0, 12, DATATBLS_MonStats2CompositLinker },
		{ "S6v", TXTFIELD_CUSTOMLINK, 0, 13, DATATBLS_MonStats2CompositLinker },
		{ "S7v", TXTFIELD_CUSTOMLINK, 0, 14, DATATBLS_MonStats2CompositLinker },
		{ "S8v", TXTFIELD_CUSTOMLINK, 0, 15, DATATBLS_MonStats2CompositLinker },
		{ "HD", TXTFIELD_BIT, 0, 232, NULL },
		{ "TR", TXTFIELD_BIT, 1, 232, NULL },
		{ "LG", TXTFIELD_BIT, 2, 232, NULL },
		{ "RA", TXTFIELD_BIT, 3, 232, NULL },
		{ "LA", TXTFIELD_BIT, 4, 232, NULL },
		{ "RH", TXTFIELD_BIT, 5, 232, NULL },
		{ "LH", TXTFIELD_BIT, 6, 232, NULL },
		{ "SH", TXTFIELD_BIT, 7, 232, NULL },
		{ "S1", TXTFIELD_BIT, 8, 232, NULL },
		{ "S2", TXTFIELD_BIT, 9, 232, NULL },
		{ "S3", TXTFIELD_BIT, 10, 232, NULL },
		{ "S4", TXTFIELD_BIT, 11, 232, NULL },
		{ "S5", TXTFIELD_BIT, 12, 232, NULL },
		{ "S6", TXTFIELD_BIT, 13, 232, NULL },
		{ "S7", TXTFIELD_BIT, 14, 232, NULL },
		{ "S8", TXTFIELD_BIT, 15, 232, NULL },
		{ "TotalPieces", TXTFIELD_BYTE, 0, 236, NULL },
		{ "mDT", TXTFIELD_BIT, 0, 240, NULL },
		{ "mNU", TXTFIELD_BIT, 1, 240, NULL },
		{ "mWL", TXTFIELD_BIT, 2, 240, NULL },
		{ "mGH", TXTFIELD_BIT, 3, 240, NULL },
		{ "mA1", TXTFIELD_BIT, 4, 240, NULL },
		{ "mA2", TXTFIELD_BIT, 5, 240, NULL },
		{ "mBL", TXTFIELD_BIT, 6, 240, NULL },
		{ "mSC", TXTFIELD_BIT, 7, 240, NULL },
		{ "mS1", TXTFIELD_BIT, 8, 240, NULL },
		{ "mS2", TXTFIELD_BIT, 9, 240, NULL },
		{ "mS3", TXTFIELD_BIT, 10, 240, NULL },
		{ "mS4", TXTFIELD_BIT, 11, 240, NULL },
		{ "mDD", TXTFIELD_BIT, 12, 240, NULL },
		{ "mKB", TXTFIELD_BIT, 13, 240, NULL },
		{ "mSQ", TXTFIELD_BIT, 14, 240, NULL },
		{ "mRN", TXTFIELD_BIT, 15, 240, NULL },
		{ "dDT", TXTFIELD_BYTE, 0, 244, NULL },
		{ "dNU", TXTFIELD_BYTE, 0, 245, NULL },
		{ "dWL", TXTFIELD_BYTE, 0, 246, NULL },
		{ "dGH", TXTFIELD_BYTE, 0, 247, NULL },
		{ "dA1", TXTFIELD_BYTE, 0, 248, NULL },
		{ "dA2", TXTFIELD_BYTE, 0, 249, NULL },
		{ "dBL", TXTFIELD_BYTE, 0, 250, NULL },
		{ "dSC", TXTFIELD_BYTE, 0, 251, NULL },
		{ "dS1", TXTFIELD_BYTE, 0, 252, NULL },
		{ "dS2", TXTFIELD_BYTE, 0, 253, NULL },
		{ "dS3", TXTFIELD_BYTE, 0, 254, NULL },
		{ "dS4", TXTFIELD_BYTE, 0, 255, NULL },
		{ "dDD", TXTFIELD_BYTE, 0, 256, NULL },
		{ "dKB", TXTFIELD_BYTE, 0, 257, NULL },
		{ "dSQ", TXTFIELD_BYTE, 0, 258, NULL },
		{ "dRN", TXTFIELD_BYTE, 0, 259, NULL },
		{ "A1mv", TXTFIELD_BIT, 4, 260, NULL },
		{ "A2mv", TXTFIELD_BIT, 5, 260, NULL },
		{ "SCmv", TXTFIELD_BIT, 7, 260, NULL },
		{ "S1mv", TXTFIELD_BIT, 8, 260, NULL },
		{ "S2mv", TXTFIELD_BIT, 9, 260, NULL },
		{ "S3mv", TXTFIELD_BIT, 10, 260, NULL },
		{ "S4mv", TXTFIELD_BIT, 11, 260, NULL },
		{ "noGfxHitTest", TXTFIELD_BIT, 0, 4, NULL },
		{ "htTop", TXTFIELD_WORD, 0, 270, NULL },
		{ "htLeft", TXTFIELD_WORD, 0, 272, NULL },
		{ "htWidth", TXTFIELD_WORD, 0, 274, NULL },
		{ "htHeight", TXTFIELD_WORD, 0, 276, NULL },
		{ "restore", TXTFIELD_BYTE, 0, 304, NULL },
		{ "automapCel", TXTFIELD_DWORD, 0, 280, NULL },
		{ "noMap", TXTFIELD_BIT, 1, 4, NULL },
		{ "noOvly", TXTFIELD_BIT, 2, 4, NULL },
		{ "isSel", TXTFIELD_BIT, 3, 4, NULL },
		{ "alSel", TXTFIELD_BIT, 4, 4, NULL },
		{ "noSel", TXTFIELD_BIT, 5, 4, NULL },
		{ "shiftSel", TXTFIELD_BIT, 6, 4, NULL },
		{ "corpseSel", TXTFIELD_BIT, 7, 4, NULL },
		{ "revive", TXTFIELD_BIT, 8, 4, NULL },
		{ "isAtt", TXTFIELD_BIT, 9, 4, NULL },
		{ "large", TXTFIELD_BIT, 11, 4, NULL },
		{ "small", TXTFIELD_BIT, 10, 4, NULL },
		{ "soft", TXTFIELD_BIT, 12, 4, NULL },
		{ "critter", TXTFIELD_BIT, 13, 4, NULL },
		{ "inert", TXTFIELD_BIT, 17, 4, NULL },
		{ "objCol", TXTFIELD_BIT, 18, 4, NULL },
		{ "deadCol", TXTFIELD_BIT, 19, 4, NULL },
		{ "unflatDead", TXTFIELD_BIT, 20, 4, NULL },
		{ "shadow", TXTFIELD_BIT, 14, 4, NULL },
		{ "noUniqueShift", TXTFIELD_BIT, 15, 4, NULL },
		{ "compositeDeath", TXTFIELD_BIT, 16, 4, NULL },
		{ "localBlood", TXTFIELD_BYTE, 0, 284, NULL },
		{ "bleed", TXTFIELD_BYTE, 0, 285, NULL },
		{ "light", TXTFIELD_BYTE, 0, 286, NULL },
		{ "light-r", TXTFIELD_BYTE, 0, 287, NULL },
		{ "light-g", TXTFIELD_BYTE, 0, 288, NULL },
		{ "light-b", TXTFIELD_BYTE, 0, 289, NULL },
		{ "Utrans", TXTFIELD_BYTE, 0, 290, NULL },
		{ "Utrans(N)", TXTFIELD_BYTE, 0, 291, NULL },
		{ "Utrans(H)", TXTFIELD_BYTE, 0, 292, NULL },
		{ "Heart", TXTFIELD_RAW, 0, 296, NULL },
		{ "BodyPart", TXTFIELD_RAW, 0, 300, NULL },
		{ "InfernoLen", TXTFIELD_BYTE, 0, 264, NULL },
		{ "InfernoAnim", TXTFIELD_BYTE, 0, 265, NULL },
		{ "InfernoRollback", TXTFIELD_BYTE, 0, 266, NULL },
		{ "ResurrectMode", TXTFIELD_CODETOBYTE, 0, 267, &gpDataTables.pMonModeLinker },
		{ "ResurrectSkill", TXTFIELD_NAMETOWORD, 0, 268, &gpDataTables.pSkillsLinker },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonStats2Linker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pMonStats2Txt = (D2MonStats2Txt*)DATATBLS_CompileTxt(pMemPool, "monstats2", pTbl, &gpDataTables.nMonStats2TxtRecordCount, sizeof(D2MonStats2Txt));
}

//D2Common.0x6FD6D660
int __fastcall DATATBLS_CheckNestedMonsterTypes(int nMonType1, int nMonType2)
{
	D2MonTypeTxt* pMonTypeTxtRecord = NULL;
	int nMonsterType = 0;
	int nIndex = 0;
	int nParentMonsterTypes[129] = {};

	if (nMonType2 <= 0)
	{
		return 0;
	}

	if (nMonType1 > 0 && nMonType1 < gpDataTables.nMonTypeTxtRecordCount)
	{
		nParentMonsterTypes[1] = nMonType1;
		nIndex = 1;
		while (1)
		{
			nMonsterType = nParentMonsterTypes[nIndex--];
			if (nMonType2 == nMonsterType)
			{
				return 1;
			}

			if (nMonsterType >= gpDataTables.nMonTypeTxtRecordCount)
			{
				break;
			}

			if (nIndex > 124)
			{
				return 0;
			}

			pMonTypeTxtRecord = &gpDataTables.pMonTypeTxt[nMonsterType];

			if (pMonTypeTxtRecord->nEquiv[0] > 0)
			{
				nParentMonsterTypes[++nIndex] = pMonTypeTxtRecord->nEquiv[0];
				if (pMonTypeTxtRecord->nEquiv[1])
				{
					nParentMonsterTypes[++nIndex] = pMonTypeTxtRecord->nEquiv[1];
					if (pMonTypeTxtRecord->nEquiv[2])
					{
						nParentMonsterTypes[++nIndex] = pMonTypeTxtRecord->nEquiv[2];
					}
				}
			}

			if (!nIndex)
			{
				return nIndex;
			}
		}

		FOG_WriteToLogFile("Invalid monster type at line %d of file %s", __LINE__, __FILE__);
	}

	return 0;
}

//D2Common.0x6FD6D730
void __fastcall DATATBLS_LoadMonTypeTxt(void* pMemPool)
{
	DWORD* pMonTypeNest = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "type", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pMonTypeLinker },
		{ "equiv1", TXTFIELD_NAMETOWORD, 0, 2, &gpDataTables.pMonTypeLinker },
		{ "equiv2", TXTFIELD_NAMETOWORD, 0, 4, &gpDataTables.pMonTypeLinker },
		{ "equiv3", TXTFIELD_NAMETOWORD, 0, 6, &gpDataTables.pMonTypeLinker },
		{ "strsing", TXTFIELD_KEYTOWORD, 0, 8, DATATBLS_GetStringIdFromReferenceString },
		{ "strplur", TXTFIELD_KEYTOWORD, 0, 10, DATATBLS_GetStringIdFromReferenceString },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonTypeLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pMonTypeTxt = (D2MonTypeTxt*)DATATBLS_CompileTxt(pMemPool, "montype", pTbl, &gpDataTables.nMonTypeTxtRecordCount, sizeof(D2MonTypeTxt));

	gpDataTables.nMonTypeIndex = (gpDataTables.nMonTypeTxtRecordCount + 31) / 32;
	gpDataTables.pMonTypeNest = (DWORD*)FOG_AllocServerMemory(NULL, sizeof(DWORD) * gpDataTables.nMonTypeTxtRecordCount * gpDataTables.nMonTypeIndex, __FILE__, __LINE__, 0);
	memset(gpDataTables.pMonTypeNest, 0x00, sizeof(DWORD) * gpDataTables.nMonTypeTxtRecordCount * gpDataTables.nMonTypeIndex);

	for (int i = 0; i < gpDataTables.nMonTypeTxtRecordCount; ++i)
	{
		pMonTypeNest = &gpDataTables.pMonTypeNest[gpDataTables.nMonTypeIndex * i];

		for (int j = 0; j < gpDataTables.nMonTypeTxtRecordCount; ++j)
		{
			if (DATATBLS_CheckNestedMonsterTypes(i, j))
			{
				pMonTypeNest[j >> 5] |= gdwBitMasks[j & 31];
			}
		}
	}
}

//D2Common.0x6FD6D910
void __fastcall DATATBLS_UnloadMonTypeTxt()
{
	if (gpDataTables.pMonTypeTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonTypeTxt);
		FOG_FreeLinker(gpDataTables.pMonTypeLinker);
		FOG_FreeServerMemory(NULL, gpDataTables.pMonTypeNest, __FILE__, __LINE__, 0);
		gpDataTables.pMonTypeTxt = NULL;
		gpDataTables.pMonTypeLinker = NULL;
	}
}

//D2Common.0x6FD6D960
void __fastcall DATATBLS_LoadMonPropTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Id", TXTFIELD_NAMETOINDEX2, 0, 0, &gpDataTables.pMonPropLinker },
		{ "prop1", TXTFIELD_NAMETODWORD, 0, 4, &gpDataTables.pPropertiesLinker },
		{ "par1", TXTFIELD_DWORD, 0, 8, NULL },
		{ "chance1", TXTFIELD_BYTE, 0, 292, NULL },
		{ "min1", TXTFIELD_DWORD, 0, 12, NULL },
		{ "max1", TXTFIELD_DWORD, 0, 16, NULL },
		{ "prop2", TXTFIELD_NAMETODWORD, 0, 20, &gpDataTables.pPropertiesLinker },
		{ "chance2", TXTFIELD_BYTE, 0, 293, NULL },
		{ "par2", TXTFIELD_DWORD, 0, 24, NULL },
		{ "min2", TXTFIELD_DWORD, 0, 28, NULL },
		{ "max2", TXTFIELD_DWORD, 0, 32, NULL },
		{ "prop3", TXTFIELD_NAMETODWORD, 0, 36, &gpDataTables.pPropertiesLinker },
		{ "chance3", TXTFIELD_BYTE, 0, 294, NULL },
		{ "par3", TXTFIELD_DWORD, 0, 40, NULL },
		{ "min3", TXTFIELD_DWORD, 0, 44, NULL },
		{ "max3", TXTFIELD_DWORD, 0, 48, NULL },
		{ "prop4", TXTFIELD_NAMETODWORD, 0, 52, &gpDataTables.pPropertiesLinker },
		{ "chance4", TXTFIELD_BYTE, 0, 295, NULL },
		{ "par4", TXTFIELD_DWORD, 0, 56, NULL },
		{ "min4", TXTFIELD_DWORD, 0, 60, NULL },
		{ "max4", TXTFIELD_DWORD, 0, 64, NULL },
		{ "prop5", TXTFIELD_NAMETODWORD, 0, 68, &gpDataTables.pPropertiesLinker },
		{ "chance5", TXTFIELD_BYTE, 0, 296, NULL },
		{ "par5", TXTFIELD_DWORD, 0, 72, NULL },
		{ "min5", TXTFIELD_DWORD, 0, 76, NULL },
		{ "max5", TXTFIELD_DWORD, 0, 80, NULL },
		{ "prop6", TXTFIELD_NAMETODWORD, 0, 84, &gpDataTables.pPropertiesLinker },
		{ "chance6", TXTFIELD_BYTE, 0, 297, NULL },
		{ "par6", TXTFIELD_DWORD, 0, 88, NULL },
		{ "min6", TXTFIELD_DWORD, 0, 92, NULL },
		{ "max6", TXTFIELD_DWORD, 0, 96, NULL },
		{ "prop1 (N)", TXTFIELD_NAMETODWORD, 0, 100, &gpDataTables.pPropertiesLinker },
		{ "chance1 (N)", TXTFIELD_BYTE, 0, 298, NULL },
		{ "par1 (N)", TXTFIELD_DWORD, 0, 104, NULL },
		{ "min1 (N)", TXTFIELD_DWORD, 0, 108, NULL },
		{ "max1 (N)", TXTFIELD_DWORD, 0, 112, NULL },
		{ "prop2 (N)", TXTFIELD_NAMETODWORD, 0, 116, &gpDataTables.pPropertiesLinker },
		{ "chance2 (N)", TXTFIELD_BYTE, 0, 299, NULL },
		{ "par2 (N)", TXTFIELD_DWORD, 0, 120, NULL },
		{ "min2 (N)", TXTFIELD_DWORD, 0, 124, NULL },
		{ "max2 (N)", TXTFIELD_DWORD, 0, 128, NULL },
		{ "prop3 (N)", TXTFIELD_NAMETODWORD, 0, 132, &gpDataTables.pPropertiesLinker },
		{ "chance3 (N)", TXTFIELD_BYTE, 0, 300, NULL },
		{ "par3 (N)", TXTFIELD_DWORD, 0, 136, NULL },
		{ "min3 (N)", TXTFIELD_DWORD, 0, 140, NULL },
		{ "max3 (N)", TXTFIELD_DWORD, 0, 144, NULL },
		{ "prop4 (N)", TXTFIELD_NAMETODWORD, 0, 148, &gpDataTables.pPropertiesLinker },
		{ "chance4 (N)", TXTFIELD_BYTE, 0, 301, NULL },
		{ "par4 (N)", TXTFIELD_DWORD, 0, 152, NULL },
		{ "min4 (N)", TXTFIELD_DWORD, 0, 156, NULL },
		{ "max4 (N)", TXTFIELD_DWORD, 0, 160, NULL },
		{ "prop5 (N)", TXTFIELD_NAMETODWORD, 0, 164, &gpDataTables.pPropertiesLinker },
		{ "chance5 (N)", TXTFIELD_BYTE, 0, 302, NULL },
		{ "par5 (N)", TXTFIELD_DWORD, 0, 168, NULL },
		{ "min5 (N)", TXTFIELD_DWORD, 0, 172, NULL },
		{ "max5 (N)", TXTFIELD_DWORD, 0, 176, NULL },
		{ "prop6 (N)", TXTFIELD_NAMETODWORD, 0, 180, &gpDataTables.pPropertiesLinker },
		{ "chance6 (N)", TXTFIELD_BYTE, 0, 303, NULL },
		{ "par6 (N)", TXTFIELD_DWORD, 0, 184, NULL },
		{ "min6 (N)", TXTFIELD_DWORD, 0, 188, NULL },
		{ "max6 (N)", TXTFIELD_DWORD, 0, 192, NULL },
		{ "prop1 (H)", TXTFIELD_NAMETODWORD, 0, 196, &gpDataTables.pPropertiesLinker },
		{ "chance1 (H)", TXTFIELD_BYTE, 0, 304, NULL },
		{ "par1 (H)", TXTFIELD_DWORD, 0, 200, NULL },
		{ "min1 (H)", TXTFIELD_DWORD, 0, 204, NULL },
		{ "max1 (H)", TXTFIELD_DWORD, 0, 208, NULL },
		{ "prop2 (H)", TXTFIELD_NAMETODWORD, 0, 212, &gpDataTables.pPropertiesLinker },
		{ "chance2 (H)", TXTFIELD_BYTE, 0, 305, NULL },
		{ "par2 (H)", TXTFIELD_DWORD, 0, 216, NULL },
		{ "min2 (H)", TXTFIELD_DWORD, 0, 220, NULL },
		{ "max2 (H)", TXTFIELD_DWORD, 0, 224, NULL },
		{ "prop3 (H)", TXTFIELD_NAMETODWORD, 0, 228, &gpDataTables.pPropertiesLinker },
		{ "chance3 (H)", TXTFIELD_BYTE, 0, 306, NULL },
		{ "par3 (H)", TXTFIELD_DWORD, 0, 232, NULL },
		{ "min3 (H)", TXTFIELD_DWORD, 0, 236, NULL },
		{ "max3 (H)", TXTFIELD_DWORD, 0, 240, NULL },
		{ "prop4 (H)", TXTFIELD_NAMETODWORD, 0, 244, &gpDataTables.pPropertiesLinker },
		{ "chance4 (H)", TXTFIELD_BYTE, 0, 307, NULL },
		{ "par4 (H)", TXTFIELD_DWORD, 0, 248, NULL },
		{ "min4 (H)", TXTFIELD_DWORD, 0, 252, NULL },
		{ "max4 (H)", TXTFIELD_DWORD, 0, 256, NULL },
		{ "prop5 (H)", TXTFIELD_NAMETODWORD, 0, 260, &gpDataTables.pPropertiesLinker },
		{ "chance5 (H)", TXTFIELD_BYTE, 0, 308, NULL },
		{ "par5 (H)", TXTFIELD_DWORD, 0, 264, NULL },
		{ "min5 (H)", TXTFIELD_DWORD, 0, 268, NULL },
		{ "max5 (H)", TXTFIELD_DWORD, 0, 272, NULL },
		{ "prop6 (H)", TXTFIELD_NAMETODWORD, 0, 276, &gpDataTables.pPropertiesLinker },
		{ "chance6 (H)", TXTFIELD_BYTE, 0, 309, NULL },
		{ "par6 (H)", TXTFIELD_DWORD, 0, 280, NULL },
		{ "min6 (H)", TXTFIELD_DWORD, 0, 284, NULL },
		{ "max6 (H)", TXTFIELD_DWORD, 0, 288, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonPropLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pMonPropTxt = (D2MonPropTxt*)DATATBLS_CompileTxt(pMemPool, "monprop", pTbl, &gpDataTables.nMonPropTxtRecordCount, sizeof(D2MonPropTxt));
}

//D2Common.0x6FD6E8E0
void __fastcall DATATBLS_LoadMonLvlTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "AC", TXTFIELD_DWORD, 0, 0, NULL },
		{ "AC(N)", TXTFIELD_DWORD, 0, 4, NULL },
		{ "AC(H)", TXTFIELD_DWORD, 0, 8, NULL },
		{ "TH", TXTFIELD_DWORD, 0, 24, NULL },
		{ "TH(N)", TXTFIELD_DWORD, 0, 28, NULL },
		{ "TH(H)", TXTFIELD_DWORD, 0, 32, NULL },
		{ "HP", TXTFIELD_DWORD, 0, 48, NULL },
		{ "HP(N)", TXTFIELD_DWORD, 0, 52, NULL },
		{ "HP(H)", TXTFIELD_DWORD, 0, 56, NULL },
		{ "DM", TXTFIELD_DWORD, 0, 72, NULL },
		{ "DM(N)", TXTFIELD_DWORD, 0, 76, NULL },
		{ "DM(H)", TXTFIELD_DWORD, 0, 80, NULL },
		{ "XP", TXTFIELD_DWORD, 0, 96, NULL },
		{ "XP(N)", TXTFIELD_DWORD, 0, 100, NULL },
		{ "XP(H)", TXTFIELD_DWORD, 0, 104, NULL },
		{ "L-AC", TXTFIELD_DWORD, 0, 12, NULL },
		{ "L-AC(N)", TXTFIELD_DWORD, 0, 16, NULL },
		{ "L-AC(H)", TXTFIELD_DWORD, 0, 20, NULL },
		{ "L-TH", TXTFIELD_DWORD, 0, 36, NULL },
		{ "L-TH(N)", TXTFIELD_DWORD, 0, 40, NULL },
		{ "L-TH(H)", TXTFIELD_DWORD, 0, 44, NULL },
		{ "L-HP", TXTFIELD_DWORD, 0, 60, NULL },
		{ "L-HP(N)", TXTFIELD_DWORD, 0, 64, NULL },
		{ "L-HP(H)", TXTFIELD_DWORD, 0, 68, NULL },
		{ "L-DM", TXTFIELD_DWORD, 0, 84, NULL },
		{ "L-DM(N)", TXTFIELD_DWORD, 0, 88, NULL },
		{ "L-DM(H)", TXTFIELD_DWORD, 0, 92, NULL },
		{ "L-XP", TXTFIELD_DWORD, 0, 108, NULL },
		{ "L-XP(N)", TXTFIELD_DWORD, 0, 112, NULL },
		{ "L-XP(H)", TXTFIELD_DWORD, 0, 116, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonLvlTxt = (D2MonLvlTxt*)DATATBLS_CompileTxt(pMemPool, "monlvl", pTbl, &gpDataTables.nMonLvlTxtRecordCount, sizeof(D2MonLvlTxt));
}

//D2Common.0x6FD6EDE0
void __fastcall DATATBLS_MonPresetPlaceLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	int nRow = 0;

	if (pRecord)
	{
		*((BYTE*)pRecord + 1) = 0;
		*((WORD*)pRecord + 1) = 0;

		if (pSrc && *pSrc)
		{
			nRow = FOG_GetRowFromTxt(gpDataTables.pSuperUniquesLinker, pSrc, 0);
			if (nRow < 0)
			{
				nRow = FOG_GetRowFromTxt(gpDataTables.pMonStatsLinker, pSrc, 0);
				if (nRow < 0)
				{
					nRow = FOG_GetRowFromTxt(gpDataTables.pMonPlaceLinker, pSrc, 1);
					if (nRow >= 0)
					{
						*((BYTE*)pRecord + 1) = 0;
						*((WORD*)pRecord + 1) = nRow;
					}
				}
				else
				{
					*((BYTE*)pRecord + 1) = 1;
					*((WORD*)pRecord + 1) = nRow;
				}
			}
			else
			{
				*((BYTE*)pRecord + 1) = 2;
				*((WORD*)pRecord + 1) = nRow;
			}
		}
	}
}

//D2Common.0x6FD6EE60
void __fastcall DATATBLS_LoadMonPresetTxt(void* pMemPool)
{
	D2MonPresetTxt* pMonPresetTxt = NULL;
	int nRecordCount = 0;
	int nActRecords = 0;
	int nAct = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "Act", TXTFIELD_BYTE, 0, 0, NULL },
		{ "Place", TXTFIELD_CUSTOMLINK, 0, 0, DATATBLS_MonPresetPlaceLinker },
		{ "end", 0, 0, 0, NULL },
	};

	pMonPresetTxt = (D2MonPresetTxt*)DATATBLS_CompileTxt(pMemPool, "monpreset", pTbl, &nRecordCount, sizeof(D2MonPresetTxt));

	gpDataTables.pMonPresetTxt = pMonPresetTxt;
	gpDataTables.pMonPresetTxtActSections[0] = pMonPresetTxt;

	for (int i = 0; i < nRecordCount; ++i)
	{
		for (int j = nAct + 1; j < pMonPresetTxt[i].nAct; ++j)
		{
			gpDataTables.nMonPresetTxtActRecordCounts[nAct] = nActRecords;
			++nAct;
			nActRecords = 0;
			gpDataTables.pMonPresetTxtActSections[nAct] = &pMonPresetTxt[i];
		}

		++nActRecords;
	}

	gpDataTables.nMonPresetTxtActRecordCounts[nAct] = nActRecords;
}

//D2Common.0x6FD6EF30 (#11256)
D2MonPresetTxt* __stdcall DATATBLS_GetMonPresetTxtActSection(int nAct, int* pRecordCount)
{
	if (pRecordCount)
	{
		if (nAct >= 0 && nAct < 5 && gpDataTables.pMonPresetTxt)
		{
			*pRecordCount = gpDataTables.nMonPresetTxtActRecordCounts[nAct];
			return gpDataTables.pMonPresetTxtActSections[nAct];
		}
		
		*pRecordCount = 0;
	}

	return NULL;
}

//D2Common.0x6FD6EF70 (#11258)
int __stdcall DATATBLS_MapSuperUniqueId(int nType, int nSuperUnique)
{
	switch (nType)
	{
	case 0:
		return nSuperUnique + gpDataTables.nSuperUniquesTxtRecordCount + gpDataTables.nMonStatsTxtRecordCount;

	case 2:
		return nSuperUnique + gpDataTables.nMonStatsTxtRecordCount;

	default:
		return nSuperUnique;

	}
}

//D2Common.0x6FD6EFA0
void __fastcall DATATBLS_LoadSoundsTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Sound", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pSoundsLinker },
		{ "end", 0, 0, 0, NULL },
	};

	if (gpDataTables.bCompileTxt)
	{
		gpDataTables.pSoundsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		gpDataTables.pSoundsTxtCodes = (D2SoundsTxtStub*)DATATBLS_CompileTxt(pMemPool, "sounds", pTbl, &gpDataTables.nSoundsTxtCodes, sizeof(D2SoundsTxtStub));
	}
}

//D2Common.0x6FD6F020
void __fastcall DATATBLS_UnloadSoundsTxt()
{
	if (gpDataTables.pSoundsTxtCodes)
	{
		DATATBLS_UnloadBin(gpDataTables.pSoundsTxtCodes);
		gpDataTables.pSoundsTxtCodes = NULL;
		FOG_FreeLinker(gpDataTables.pSoundsLinker);
		gpDataTables.pSoundsLinker = NULL;
		gpDataTables.nSoundsTxtCodes = 0;
	}
}

//D2Common.0x6FD6F050
void __fastcall DATATBLS_LoadMonSeqTxt(void* pMemPool)
{
	int nSequence = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "sequence", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pMonSeqLinker },
		{ "mode", TXTFIELD_CODETOBYTE, 0, 2, &gpDataTables.pMonModeLinker },
		{ "frame", TXTFIELD_BYTE, 0, 3, NULL },
		{ "dir", TXTFIELD_BYTE, 0, 4, NULL },
		{ "event", TXTFIELD_BYTE, 0, 5, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonSeqLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	gpDataTables.pMonSeqTxt = (D2MonSeqTxt*)DATATBLS_CompileTxt(pMemPool, "monseq", pTbl, &gpDataTables.nMonSeqTxtRecordCount, sizeof(D2MonSeqTxt));

	if (gpDataTables.nMonSeqTxtRecordCount > 0)
	{
		gpDataTables.nMonSeqTableRecordCount = gpDataTables.pMonSeqTxt[gpDataTables.nMonSeqTxtRecordCount - 1].wSequence + 1;
		gpDataTables.pMonSeqTable = (D2SeqRecordStrc*)FOG_AllocServerMemory(NULL, sizeof(D2SeqRecordStrc) * gpDataTables.nMonSeqTableRecordCount, __FILE__, __LINE__, 0);
		memset(gpDataTables.pMonSeqTable, 0x00, sizeof(D2SeqRecordStrc) * gpDataTables.nMonSeqTableRecordCount);

		for (int i = 0; i < gpDataTables.nMonSeqTxtRecordCount; ++i)
		{
			nSequence = gpDataTables.pMonSeqTxt[i].wSequence;
			if (!gpDataTables.pMonSeqTable[nSequence].pMonSeqTxtRecord)
			{
				gpDataTables.pMonSeqTable[nSequence].pMonSeqTxtRecord = &gpDataTables.pMonSeqTxt[i];
			}

			++gpDataTables.pMonSeqTable[nSequence].unk0x04;
			++gpDataTables.pMonSeqTable[nSequence].unk0x08;
		}
	}
}

//D2Common.0x6FD6F200 (#11262)
D2SeqRecordStrc* __stdcall DATATBLS_GetMonSeqTableRecord(int nSequence)
{
	if (nSequence >= 0 && nSequence < gpDataTables.nMonSeqTableRecordCount)
	{
		return &gpDataTables.pMonSeqTable[nSequence];
	}

	return NULL;
}

//D2Common.0x6FD6F230
void __fastcall DATATBLS_LoadMonEquipTxt(void* pMemPool)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2MonEquipTxt* pMonEquipTxtRecord = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "monster", TXTFIELD_NAMETOWORD, 0, 0, &gpDataTables.pMonStatsLinker },
		{ "level", TXTFIELD_WORD, 0, 2, NULL },
		{ "oninit", TXTFIELD_BYTE, 0, 4, NULL },
		{ "item1", TXTFIELD_RAW, 0, 8, &gpDataTables.pItemsLinker },
		{ "loc1", TXTFIELD_CODETOBYTE, 0, 20, &gpDataTables.pBodyLocsLinker },
		{ "mod1", TXTFIELD_BYTE, 0, 23, NULL },
		{ "item2", TXTFIELD_RAW, 0, 12, &gpDataTables.pItemsLinker },
		{ "loc2", TXTFIELD_CODETOBYTE, 0, 21, &gpDataTables.pBodyLocsLinker },
		{ "mod2", TXTFIELD_BYTE, 0, 24, NULL },
		{ "item3", TXTFIELD_RAW, 0, 16, &gpDataTables.pItemsLinker },
		{ "loc3", TXTFIELD_CODETOBYTE, 0, 22, &gpDataTables.pBodyLocsLinker },
		{ "mod3", TXTFIELD_BYTE, 0, 25, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pMonEquipTxt = (D2MonEquipTxt*)DATATBLS_CompileTxt(pMemPool, "monequip", pTbl, &gpDataTables.nMonEquipTxtRecordCount, sizeof(D2MonEquipTxt));

	if (gpDataTables.nMonEquipTxtRecordCount >= 32767)
	{
		FOG_10025("Monequip.txt exceeded maximum number of allowable rows", __FILE__, __LINE__);
	}

	for (int i = 0; i < gpDataTables.nMonStatsTxtRecordCount; ++i)
	{
		gpDataTables.pMonStatsTxt[i].nMonEquipTxtRecordId = -1;
	}

	for (int i = 0; i < gpDataTables.nMonEquipTxtRecordCount; ++i)
	{
		pMonEquipTxtRecord = &gpDataTables.pMonEquipTxt[i];
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pMonEquipTxtRecord->nMonster);
		if (pMonStatsTxtRecord)
		{
			if (pMonStatsTxtRecord->nMonEquipTxtRecordId < 0)
			{
				pMonStatsTxtRecord->nMonEquipTxtRecordId = i;
			}

			for (int j = 0; j < 3; ++j)
			{
				if (pMonEquipTxtRecord->nLoc[j] <= BODYLOC_NONE || pMonEquipTxtRecord->nLoc[j] >= BODYLOC_SWRARM
					|| pMonEquipTxtRecord->dwItem[j] != '    ' && FOG_GetLinkIndex(gpDataTables.pItemsLinker, pMonEquipTxtRecord->dwItem[j], 1) < 0)
				{
					pMonEquipTxtRecord->nLoc[j] = BODYLOC_NONE;
				}
			}
		}
	}
}

//D2Common.0x6FD6F580
void __fastcall DATATBLS_LoadSomeMonsterTxts(void* pMemPool)
{
	static const char* szTreasureClassVariants[] = { "A", "B", "C" };
	static const char* szDifficulties[] = { "", " (N)", " (H)" };

	D2TCExShortStrc** ppTCEx = NULL;
	int nTxtRow = 0;

	char szChest[256] = {};

	D2BinFieldStrc pTbl[] =
	{
		{ "Name", TXTFIELD_KEYTOWORD, 0, 0, DATATBLS_GetStringIdFromReferenceString },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pUniqueTitleTxt = (D2UniqueTitleTxt*)DATATBLS_CompileTxt(pMemPool, "uniquetitle", pTbl, &gpDataTables.nUniqueTitleTxtRecordCount, sizeof(D2UniqueTitleTxt));
	gpDataTables.pUniquePrefixTxt = (D2UniquePrefixTxt*)DATATBLS_CompileTxt(pMemPool, "uniqueprefix", pTbl, &gpDataTables.nUniquePrefixTxtRecordCount, sizeof(D2UniquePrefixTxt));
	gpDataTables.pUniqueSuffixTxt = (D2UniqueSuffixTxt*)DATATBLS_CompileTxt(pMemPool, "uniquesuffix", pTbl, &gpDataTables.nUniqueSuffixTxtRecordCount, sizeof(D2UniqueSuffixTxt));
	gpDataTables.pUniqueAppellationTxt = (D2UniqueAppellationTxt*)DATATBLS_CompileTxt(pMemPool, "uniqueappellation", pTbl, &gpDataTables.nUniqueAppellationTxtRecordCount, sizeof(D2UniqueAppellationTxt));

	DATATBLS_LoadMonLvlTxt(pMemPool);

	gpDataTables.pTreasureClassExLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);

	DATATBLS_CreateItemTypeTreasureClasses();
	DATATBLS_LoadTreasureClassExTxt(pMemPool);

	ppTCEx = gpDataTables.pChestTreasureClasses;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				wsprintfA(szChest, "Act %d%s Chest %s", j + 1, szDifficulties[i], szTreasureClassVariants[k]);

				nTxtRow = FOG_GetRowFromTxt(gpDataTables.pTreasureClassExLinker, szChest, 0);
				if (nTxtRow >= 0)
				{
					*ppTCEx = &gpDataTables.pTreasureClassEx[nTxtRow];
				}
				else
				{
					*ppTCEx = 0;
				}

				++ppTCEx;
			}
		}
	}

	D2_ASSERT(gpDataTables.nTreasureClassEx < USHRT_MAX);

	DATATBLS_LoadMonStats2Txt(pMemPool);
	DATATBLS_LoadMonPropTxt(pMemPool);
	DATATBLS_LoadMonSoundsTxt(pMemPool);
	DATATBLS_LoadMonSeqTxt(pMemPool);
	DATATBLS_LoadMonStatsTxt(pMemPool);
	DATATBLS_LoadMonUModTxt(pMemPool);
	DATATBLS_LoadSuperUniquesTxt(pMemPool);
	DATATBLS_LoadMonPresetTxt(pMemPool);
	DATATBLS_LoadHirelingTxt(pMemPool);
	DATATBLS_LoadNpcTxt(pMemPool);
	DATATBLS_LoadMonEquipTxt(pMemPool);
}

//D2Common.0x6FD6F810
void __fastcall DATATBLS_CreateItemTypeTreasureClasses()
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2TCExShortStrc* pTCExTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nProbability = 0;
	int nItemType = 0;
	int nLevel = 0;
	int nTypes = 0;

	char v37[8] = {};
	char dest[32] = {};

	FOG_10216_AddRecordToLinkingTable(gpDataTables.pTreasureClassExLinker, &gpDataTables.szDefaultString);

	if (!(gpDataTables.nTreasureClassEx % 16))
	{
		gpDataTables.pTreasureClassEx = (D2TCExShortStrc*)FOG_ReallocServerMemory(NULL, gpDataTables.pTreasureClassEx, sizeof(D2TCExShortStrc) * (gpDataTables.nTreasureClassEx + 16), __FILE__, __LINE__, 0);
	}
	memset(&gpDataTables.pTreasureClassEx[gpDataTables.nTreasureClassEx], 0x00, sizeof(gpDataTables.pTreasureClassEx[gpDataTables.nTreasureClassEx]));
	++gpDataTables.nTreasureClassEx;

	for (int i = 0; i < gpDataTables.nItemTypesTxtRecordCount; ++i)
	{
		if (!&gpDataTables.pItemTypesTxt[i])
		{
			break;
		}

		if (gpDataTables.pItemTypesTxt[i].nTreasureClass)
		{
			++gpDataTables.nTreasureClassItemTypes;

			nLevel = 3;
			do
			{
				if (FOG_GetStringFromLinkIndex(gpDataTables.pItemTypesLinker, i, v37))
				{
					wsprintfA(dest, "%s%d", v37, nLevel);
					FOG_10216_AddRecordToLinkingTable(gpDataTables.pTreasureClassExLinker, dest);

					if (!(gpDataTables.nTreasureClassEx % 16))
					{
						gpDataTables.pTreasureClassEx = (D2TCExShortStrc*)FOG_ReallocServerMemory(NULL, gpDataTables.pTreasureClassEx, sizeof(D2TCExShortStrc) * (gpDataTables.nTreasureClassEx + 16), __FILE__, __LINE__, 0);
					}
					memset(&gpDataTables.pTreasureClassEx[gpDataTables.nTreasureClassEx], 0x00, sizeof(gpDataTables.pTreasureClassEx[gpDataTables.nTreasureClassEx]));

					pTCExTxtRecord = &gpDataTables.pTreasureClassEx[gpDataTables.nTreasureClassEx++];

					if (pTCExTxtRecord)
					{
						pTCExTxtRecord->nPicks = 1;
						pTCExTxtRecord->nGroup = 0;
						pTCExTxtRecord->nLevel = nLevel - 3;

						for (int j = 0; j < gpDataTables.pItemDataTables.nItemsTxtRecordCount; ++j)
						{
							pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(j);
							if (pItemsTxtRecord && !pItemsTxtRecord->nQuest && pItemsTxtRecord->nSpawnable && ITEMS_CheckItemTypeIdByItemId(j, i) && (i == ITEMTYPE_MISSILE_POTION || !ITEMS_CheckItemTypeIdByItemId(j, ITEMTYPE_MISSILE_POTION)))
							{
								if (pItemsTxtRecord->nLevel > nLevel - 3 && pItemsTxtRecord->nLevel <= nLevel)
								{
									nItemType = pItemsTxtRecord->wType[0];
									if (nItemType >= 0 && nItemType < gpDataTables.nItemTypesTxtRecordCount)
									{
										pItemTypesTxtRecord = &gpDataTables.pItemTypesTxt[nItemType];
										if (pItemTypesTxtRecord)
										{
											nProbability = pItemTypesTxtRecord->nRarity;
											if (nProbability <= 1)
											{
												nProbability = 1;
											}

											nTypes = pTCExTxtRecord->nTypes;
											DATATBLS_ReallocTCExInfo(pTCExTxtRecord, nTypes + 1);

											pTCExTxtRecord->pInfo[nTypes].nClassic = pTCExTxtRecord->nClassic;
											pTCExTxtRecord->pInfo[nTypes].nProb = pTCExTxtRecord->nProb;
											pTCExTxtRecord->pInfo[nTypes].nItemId = j;

											if (pItemsTxtRecord->wVersion >= 100)
											{
												pTCExTxtRecord->pInfo[nTypes].nFlags |= 0x10;
											}

											DATATBLS_UpdateTreasureClassProbabilities(pTCExTxtRecord, &pTCExTxtRecord->pInfo[nTypes], nProbability, pItemsTxtRecord->wVersion >= 100);
										}
									}
								}
							}
						}
					}
				}
				nLevel += 3;
			}
			while (nLevel <= 96);
		}
	}
}

//D2Common.0x6FD6FBB0
void __fastcall DATATBLS_UnloadSomeMonsterTxts()
{
	if (gpDataTables.pMonEquipTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonEquipTxt);
		gpDataTables.nMonEquipTxtRecordCount = 0;
	}

	DATATBLS_UnloadBin(gpDataTables.pNpcTxt);

	DATATBLS_UnloadBin(gpDataTables.pHirelingTxt);
	gpDataTables.nHirelingTxtRecordCount = 0;
	for (int i = 0; i < 256; ++i)
	{
		gpDataTables.nClassicHirelingStartRecordIds[i] = -1;
		gpDataTables.nExpansionHirelingStartRecordIds[i] = -1;
	}

	if (gpDataTables.pMonPresetTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonPresetTxt);
	}
	for (int i = 0; i < 5; ++i)
	{
		gpDataTables.pMonPresetTxtActSections[i] = NULL;
		gpDataTables.nMonPresetTxtActRecordCounts[i] = 0;
	}

	if (gpDataTables.pSuperUniquesLinker)
	{
		FOG_FreeLinker(gpDataTables.pSuperUniquesLinker);
	}
	if (gpDataTables.pSuperUniquesTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pSuperUniquesTxt);
		gpDataTables.pSuperUniquesTxt = NULL;
	}
	gpDataTables.nSuperUniquesTxtRecordCount = 0;

	if (gpDataTables.pMonUModLinker)
	{
		FOG_FreeLinker(gpDataTables.pMonUModLinker);
		gpDataTables.pMonUModLinker = NULL;
	}
	if (gpDataTables.pMonUModTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonUModTxt);
		gpDataTables.pMonUModTxt = NULL;
	}
	gpDataTables.nMonUModTxtRecordCount = 0;

	if (gpDataTables.pMonStatsTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonStatsTxt);
		FOG_FreeLinker(gpDataTables.pMonStatsLinker);
	}
	gpDataTables.pMonStatsTxt = NULL;

	if (gpDataTables.pMonStats2Txt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonStats2Txt);
		FOG_FreeLinker(gpDataTables.pMonStats2Linker);
	}

	if (gpDataTables.pMonPropTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonPropTxt);
		FOG_FreeLinker(gpDataTables.pMonPropLinker);
	}
	gpDataTables.nMonPropTxtRecordCount = 0;

	if (gpDataTables.pMonSoundsTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonSoundsTxt);
		FOG_FreeLinker(gpDataTables.pMonSoundsLinker);
		gpDataTables.nMonSoundsTxtRecordCount = 0;
	}

	if (gpDataTables.pMonSeqTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonSeqTxt);
		FOG_FreeLinker(gpDataTables.pMonSeqLinker);
		gpDataTables.nMonSeqTxtRecordCount = 0;
	}
	if (gpDataTables.pMonSeqTable)
	{
		FOG_FreeServerMemory(NULL, gpDataTables.pMonSeqTable, __FILE__, __LINE__, 0);
		gpDataTables.nMonSeqTableRecordCount = 0;
	}

	FOG_FreeLinker(gpDataTables.pTreasureClassExLinker);
	for (int i = 0; i < gpDataTables.nTreasureClassEx; ++i)
	{
		if (gpDataTables.pTreasureClassEx[i].pInfo)
		{
			FOG_FreeServerMemory(NULL, gpDataTables.pTreasureClassEx[i].pInfo, __FILE__, __LINE__, 0);
		}
	}
	FOG_FreeServerMemory(NULL, gpDataTables.pTreasureClassEx, __FILE__, __LINE__, 0);
	gpDataTables.pTreasureClassEx = NULL;
	gpDataTables.nTreasureClassEx = 0;

	if (gpDataTables.pMonLvlTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pMonLvlTxt);
		gpDataTables.nMonLvlTxtRecordCount = 0;
	}

	if (gpDataTables.pUniqueTitleTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pUniqueTitleTxt);
		gpDataTables.pUniqueTitleTxt = NULL;
		gpDataTables.nUniqueTitleTxtRecordCount = 0;
	}

	if (gpDataTables.pUniquePrefixTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pUniquePrefixTxt);
		gpDataTables.pUniquePrefixTxt = NULL;
		gpDataTables.nUniquePrefixTxtRecordCount = 0;
	}

	if (gpDataTables.pUniqueSuffixTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pUniqueSuffixTxt);
		gpDataTables.pUniqueSuffixTxt = NULL;
		gpDataTables.nUniqueSuffixTxtRecordCount = 0;
	}

	if (gpDataTables.pUniqueAppellationTxt)
	{
		DATATBLS_UnloadBin(gpDataTables.pUniqueAppellationTxt);
		gpDataTables.pUniqueAppellationTxt = NULL;
		gpDataTables.nUniqueAppellationTxtRecordCount = 0;
	}
}

//Inlined at various places
D2MonStatsTxt* __fastcall DATATBLS_GetMonStatsTxtRecord(int nMonsterId)
{
	if (nMonsterId >= 0 && nMonsterId < gpDataTables.nMonStatsTxtRecordCount)
	{
		return &gpDataTables.pMonStatsTxt[nMonsterId];
	}

	return NULL;
}
