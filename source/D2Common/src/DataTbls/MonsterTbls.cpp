#include "D2DataTbls.h"

#include "D2Items.h"
#include "D2Seed.h"
#include <D2Lang.h>
#include <D2BitManip.h>
#include <Units/Units.h>
#include <D2Monsters.h>

//Inlined in some functions
uint32_t __fastcall DATATBLS_StringToCode(char* szText)
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

	return *(uint32_t*)dwCode;
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
				pMonStatsTxtRecord->nSequence[nOffset] = FOG_GetRowFromTxt(sgptDataTables->pMonSeqLinker, pSrc, 1);
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

			nMode = FOG_GetLinkIndex(sgptDataTables->pMonModeLinker, DATATBLS_StringToCode(szMode), 1);
			pMonStatsTxtRecord->nSkillMode[nOffset] = nMode;

			if (nMode < 0 || nMode == MONMODE_SEQUENCE)
			{
				pMonStatsTxtRecord->nSkillMode[nOffset] = MONMODE_SEQUENCE;
				pMonStatsTxtRecord->nSequence[nOffset] = FOG_GetRowFromTxt(sgptDataTables->pMonSeqLinker, pSrc, 1);
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
		{ "Id", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pMonStatsLinker },
		{ "BaseId", TXTFIELD_NAMETOWORD, 0, 2, &sgptDataTables->pMonStatsLinker },
		{ "NextInClass", TXTFIELD_NAMETOWORD, 0, 4, &sgptDataTables->pMonStatsLinker },
		{ "NameStr", TXTFIELD_KEYTOWORD, 0, 6, DATATBLS_GetStringIdFromReferenceString },
		{ "DescStr", TXTFIELD_KEYTOWORD, 0, 8, DATATBLS_GetStringIdFromReferenceString },
		{ "Code", TXTFIELD_RAW, 0, 16, NULL },
		{ "TransLvl", TXTFIELD_BYTE, 0, 77, NULL },
		{ "MonSound", TXTFIELD_NAMETOWORD, 0, 20, &sgptDataTables->pMonSoundsLinker },
		{ "UMonSound", TXTFIELD_NAMETOWORD, 0, 22, &sgptDataTables->pMonSoundsLinker },
		{ "MonStatsEx", TXTFIELD_NAMETOWORD, 0, 24, &sgptDataTables->pMonStats2Linker },
		{ "MonType", TXTFIELD_NAMETOWORD, 0, 28, &sgptDataTables->pMonTypeLinker },
		{ "MonProp", TXTFIELD_NAMETOWORD, 0, 26, &sgptDataTables->pMonPropLinker },
		{ "AI", TXTFIELD_NAMETOWORD, 0, 30, &sgptDataTables->pMonAiLinker },
		{ "spawn", TXTFIELD_NAMETOWORD, 0, 32, &sgptDataTables->pMonStatsLinker },
		{ "spawnx", TXTFIELD_BYTE, 0, 34, NULL },
		{ "spawny", TXTFIELD_BYTE, 0, 35, NULL },
		{ "spawnmode", TXTFIELD_CODETOBYTE, 0, 36, &sgptDataTables->pMonModeLinker },
		{ "minion1", TXTFIELD_NAMETOWORD, 0, 38, &sgptDataTables->pMonStatsLinker },
		{ "minion2", TXTFIELD_NAMETOWORD, 0, 40, &sgptDataTables->pMonStatsLinker },
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
		{ "MissA1", TXTFIELD_NAMETOWORD, 0, 58, &sgptDataTables->pMissilesLinker },
		{ "MissA2", TXTFIELD_NAMETOWORD, 0, 60, &sgptDataTables->pMissilesLinker },
		{ "MissS1", TXTFIELD_NAMETOWORD, 0, 62, &sgptDataTables->pMissilesLinker },
		{ "MissS2", TXTFIELD_NAMETOWORD, 0, 64, &sgptDataTables->pMissilesLinker },
		{ "MissS3", TXTFIELD_NAMETOWORD, 0, 66, &sgptDataTables->pMissilesLinker },
		{ "MissS4", TXTFIELD_NAMETOWORD, 0, 68, &sgptDataTables->pMissilesLinker },
		{ "MissC", TXTFIELD_NAMETOWORD, 0, 70, &sgptDataTables->pMissilesLinker },
		{ "MissSQ", TXTFIELD_NAMETOWORD, 0, 72, &sgptDataTables->pMissilesLinker },
		{ "TreasureClass1", TXTFIELD_NAMETOWORD, 0, 134, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass2", TXTFIELD_NAMETOWORD, 0, 136, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass3", TXTFIELD_NAMETOWORD, 0, 138, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass4", TXTFIELD_NAMETOWORD, 0, 140, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass1(N)", TXTFIELD_NAMETOWORD, 0, 142, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass2(N)", TXTFIELD_NAMETOWORD, 0, 144, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass3(N)", TXTFIELD_NAMETOWORD, 0, 146, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass4(N)", TXTFIELD_NAMETOWORD, 0, 148, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass1(H)", TXTFIELD_NAMETOWORD, 0, 150, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass2(H)", TXTFIELD_NAMETOWORD, 0, 152, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass3(H)", TXTFIELD_NAMETOWORD, 0, 154, &sgptDataTables->pTreasureClassExLinker },
		{ "TreasureClass4(H)", TXTFIELD_NAMETOWORD, 0, 156, &sgptDataTables->pTreasureClassExLinker },
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
		{ "SkillDamage", TXTFIELD_NAMETOWORD, 0, 168, &sgptDataTables->pSkillsLinker },
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
		{ "El1Mode", TXTFIELD_CODETOBYTE, 0, 254, &sgptDataTables->pMonModeLinker },
		{ "El1Type", TXTFIELD_CODETOBYTE, 0, 257, &sgptDataTables->pElemTypesLinker },
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
		{ "El2Mode", TXTFIELD_CODETOBYTE, 0, 255, &sgptDataTables->pMonModeLinker },
		{ "El2Type", TXTFIELD_CODETOBYTE, 0, 258, &sgptDataTables->pElemTypesLinker },
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
		{ "El3Mode", TXTFIELD_CODETOBYTE, 0, 256, &sgptDataTables->pMonModeLinker },
		{ "El3Type", TXTFIELD_CODETOBYTE, 0, 259, &sgptDataTables->pElemTypesLinker },
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
		{ "Skill1", TXTFIELD_NAMETOWORD, 0, 368, &sgptDataTables->pSkillsLinker },
		{ "Sk1mode", TXTFIELD_CUSTOMLINK, 0, 0, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk1lvl", TXTFIELD_BYTE, 0, 408, NULL },
		{ "Skill2", TXTFIELD_NAMETOWORD, 0, 370, &sgptDataTables->pSkillsLinker },
		{ "Sk2mode", TXTFIELD_CUSTOMLINK, 0, 1, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk2lvl", TXTFIELD_BYTE, 0, 409, NULL },
		{ "Skill3", TXTFIELD_NAMETOWORD, 0, 372, &sgptDataTables->pSkillsLinker },
		{ "Sk3mode", TXTFIELD_CUSTOMLINK, 0, 2, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk3lvl", TXTFIELD_BYTE, 0, 410, NULL },
		{ "Skill4", TXTFIELD_NAMETOWORD, 0, 374, &sgptDataTables->pSkillsLinker },
		{ "Sk4mode", TXTFIELD_CUSTOMLINK, 0, 3, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk4lvl", TXTFIELD_BYTE, 0, 411, NULL },
		{ "Skill5", TXTFIELD_NAMETOWORD, 0, 376, &sgptDataTables->pSkillsLinker },
		{ "Sk5mode", TXTFIELD_CUSTOMLINK, 0, 4, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk5lvl", TXTFIELD_BYTE, 0, 412, NULL },
		{ "Skill6", TXTFIELD_NAMETOWORD, 0, 378, &sgptDataTables->pSkillsLinker },
		{ "Sk6mode", TXTFIELD_CUSTOMLINK, 0, 5, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk6lvl", TXTFIELD_BYTE, 0, 413, NULL },
		{ "Skill7", TXTFIELD_NAMETOWORD, 0, 380, &sgptDataTables->pSkillsLinker },
		{ "Sk7mode", TXTFIELD_CUSTOMLINK, 0, 6, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk7lvl", TXTFIELD_BYTE, 0, 414, NULL },
		{ "Skill8", TXTFIELD_NAMETOWORD, 0, 382, &sgptDataTables->pSkillsLinker },
		{ "Sk8mode", TXTFIELD_CUSTOMLINK, 0, 7, DATATBLS_MonStatsSkillModeLinker },
		{ "Sk8lvl", TXTFIELD_BYTE, 0, 415, NULL },
		{ "DamageRegen", TXTFIELD_DWORD, 0, 416, NULL },
		{ "SplEndDeath", TXTFIELD_BYTE, 0, 420, NULL },
		{ "SplGetModeChart", TXTFIELD_BYTE, 0, 421, NULL },
		{ "SplEndGeneric", TXTFIELD_BYTE, 0, 422, NULL },
		{ "SplClientEnd", TXTFIELD_BYTE, 0, 423, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMonStatsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pMonStatsTxt = (D2MonStatsTxt*)DATATBLS_CompileTxt(pMemPool, "monstats", pTbl, &sgptDataTables->nMonStatsTxtRecordCount, sizeof(D2MonStatsTxt));

	D2_ASSERT(sgptDataTables->nMonStatsTxtRecordCount < SHRT_MAX);

	if (sgptDataTables->nMonStatsTxtRecordCount > 0)
	{
		nCounter = 0;
		do
		{
			bFoundInChain = false;

			nChainId = 0;
			nId = sgptDataTables->pMonStatsTxt[nCounter].nBaseId;

			do
			{
				if (sgptDataTables->pMonStatsTxt[nId].nBaseId != sgptDataTables->pMonStatsTxt[nCounter].nBaseId && sgptDataTables->bCompileTxt)
				{
					szMonsterName = FOG_10255(sgptDataTables->pMonStatsLinker, sgptDataTables->pMonStatsTxt[nId].nId, 0);
					FOG_Trace("BaseId/NextInClass chain -- unexpected baseid for monster '%s' (%d)", szMonsterName);
				}

				if (nId == nCounter)
				{
					bFoundInChain = true;
					sgptDataTables->pMonStatsTxt[nCounter].nChainId = nChainId;
				}

				nNextInClass = sgptDataTables->pMonStatsTxt[nId].nNextInClass;

				++nChainId;
				if (nId == nNextInClass)
				{
					if (sgptDataTables->bCompileTxt)
					{
						szMonsterName = FOG_10255(sgptDataTables->pMonStatsLinker, sgptDataTables->pMonStatsTxt[nId].nId, 0);
						FOG_Trace("BaseId/NextInClass chain -- monster '%s' (%d) pointing to itself", szMonsterName);
					}
					break;
				}

				nId = nNextInClass;

				if (nChainId > 255)
				{
					if (sgptDataTables->bCompileTxt)
					{
						szMonsterName = FOG_10255(sgptDataTables->pMonStatsLinker, sgptDataTables->pMonStatsTxt[nNextInClass].nId, 0);
						FOG_Trace("BaseId/NextInClass chain exceeded 255 entries at monster '%s' (%d)", szMonsterName);
					}
					break;
				}
			}
			while (nNextInClass >= 0);

			if (!bFoundInChain && sgptDataTables->bCompileTxt)
			{
				szMonsterName = FOG_10255(sgptDataTables->pMonStatsLinker, sgptDataTables->pMonStatsTxt[nCounter].nId, 0);
				FOG_Trace("BaseId/NextInClass monster '%s' (%d) not found in chain", szMonsterName);
			}
			sgptDataTables->pMonStatsTxt[nCounter].nMaxChainId = nChainId;

			++nCounter;
		}
		while (nCounter < sgptDataTables->nMonStatsTxtRecordCount);
	}


	nCounter = 0;
	while (nCounter < sgptDataTables->nMonStatsTxtRecordCount)
	{
		nBaseId = sgptDataTables->pMonStatsTxt[nCounter].nBaseId;
		if (nBaseId < 0 || nBaseId >= sgptDataTables->nMonStatsTxtRecordCount)
		{
			sgptDataTables->pMonStatsTxt[nCounter].nBaseId = nCounter;
			nBaseId = nCounter;
		}

		nVelocity = *((uint32_t*)DATATBLS_GetAnimDataRecord(0, nBaseId, MONMODE_WALK, 1, 0) + 3);
		if (nBaseId != nCounter)
		{
			if (sgptDataTables->pMonStatsTxt[nBaseId].nVelocity > 0)
			{
				nVelocity = nVelocity * sgptDataTables->pMonStatsTxt[nCounter].nVelocity / (unsigned int)sgptDataTables->pMonStatsTxt[nBaseId].nVelocity;
			}
		}

		if (nVelocity <= 0)
		{
			sgptDataTables->pMonStatsTxt[nCounter].unk0x36 = 0;
		}
		else if (nVelocity >= 32767)
		{
			sgptDataTables->pMonStatsTxt[nCounter].unk0x36 = 32767;
		}
		else
		{
			sgptDataTables->pMonStatsTxt[nCounter].unk0x36 = nVelocity;
		}

		if (nCounter < 410)//Before Expansion
		{
			v21 = sgptDataTables->pMonStatsTxt[nBaseId].unk0x36;
			nRun = ((signed int)v21 - HIDWORD(v21)) >> 1;
		}
		else
		{
			nRun = DATATBLS_GetAnimDataRecord(NULL, nBaseId, MONMODE_RUN, UNIT_MONSTER, NULL)->dwAnimSpeed;
		}

		if (nBaseId != nCounter)
		{
			if (sgptDataTables->pMonStatsTxt[nBaseId].nRun > 0)
			{
				nRun = nRun * sgptDataTables->pMonStatsTxt[nCounter].nRun / (unsigned int)sgptDataTables->pMonStatsTxt[nBaseId].nRun;
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

		sgptDataTables->pMonStatsTxt[nCounter].unk0x38 = nRun;

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
		if (nLevel > sgptDataTables->nMonLvlTxtRecordCount - 1)
		{
			nLevel = sgptDataTables->nMonLvlTxtRecordCount - 1;
		}

		if (nLevel >= 0 && nLevel < sgptDataTables->nMonLvlTxtRecordCount)
		{
			pMonLvlTxtRecord = &sgptDataTables->pMonLvlTxt[nLevel];
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

	D2_ASSERT(sgptDataTables->pMonStatsTxt);

	pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
	pMonStatsTxtRecord->nVelocity = nVelocity;
}

//D2Common.0x6FD68A00
void __stdcall DATATBLS_ResetGlobalDefaultUnicodeString()
{
	sgptDataTables->wszDefault = 0;
}

//D2Common.0x6FD68A10 (#10651)
wchar_t* __fastcall DATATBLS_RollRandomUniqueTitleString(D2UnitStrc* pUnit)
{
	wchar_t* pString = NULL;
	int nRand = 0;

	if (pUnit && sgptDataTables->nUniqueTitleTxtRecordCount > 0)
	{
		if ((sgptDataTables->nUniqueTitleTxtRecordCount - 1) & sgptDataTables->nUniqueTitleTxtRecordCount)
		{
			nRand = (unsigned int)SEED_RollRandomNumber(&pUnit->pSeed) % sgptDataTables->nUniqueTitleTxtRecordCount;
		}
		else
		{
			nRand = SEED_RollRandomNumber(&pUnit->pSeed) & (sgptDataTables->nUniqueTitleTxtRecordCount - 1);
		}

		if (sgptDataTables->pUniqueTitleTxt[nRand].wStringId)
		{
			pString = D2LANG_GetStringFromTblIndex(sgptDataTables->pUniqueTitleTxt[nRand].wStringId);
			if (pString)
			{
				return pString;
			}
		}
	}

	return &sgptDataTables->wszDefault;
}

//D2Common.0x6FD68A80 (#10652)
wchar_t* __fastcall DATATBLS_RollRandomUniquePrefixString(D2UnitStrc* pUnit)
{
	wchar_t* pString = NULL;
	int nRand = 0;

	if (pUnit && sgptDataTables->nUniquePrefixTxtRecordCount > 0)
	{
		if ((sgptDataTables->nUniquePrefixTxtRecordCount - 1) & sgptDataTables->nUniquePrefixTxtRecordCount)
		{
			nRand = (unsigned int)SEED_RollRandomNumber(&pUnit->pSeed) % sgptDataTables->nUniquePrefixTxtRecordCount;
		}
		else
		{
			nRand = SEED_RollRandomNumber(&pUnit->pSeed) & (sgptDataTables->nUniquePrefixTxtRecordCount - 1);
		}

		if (sgptDataTables->pUniquePrefixTxt[nRand].wStringId)
		{
			pString = D2LANG_GetStringFromTblIndex(sgptDataTables->pUniquePrefixTxt[nRand].wStringId);
			if (pString)
			{
				return pString;
			}
		}
	}

	return &sgptDataTables->wszDefault;
}

//D2Common.0x6FD68AF0 (#10653)
wchar_t* __fastcall DATATBLS_RollRandomUniqueSuffixString(D2UnitStrc* pUnit)
{
	wchar_t* pString = NULL;
	int nRand = 0;

	if (pUnit && sgptDataTables->nUniqueSuffixTxtRecordCount > 0)
	{
		if ((sgptDataTables->nUniqueSuffixTxtRecordCount - 1) & sgptDataTables->nUniqueSuffixTxtRecordCount)
		{
			nRand = (unsigned int)SEED_RollRandomNumber(&pUnit->pSeed) % sgptDataTables->nUniqueSuffixTxtRecordCount;
		}
		else
		{
			nRand = SEED_RollRandomNumber(&pUnit->pSeed) & (sgptDataTables->nUniqueSuffixTxtRecordCount - 1);
		}

		if (sgptDataTables->pUniqueSuffixTxt[nRand].wStringId)
		{
			pString = D2LANG_GetStringFromTblIndex(sgptDataTables->pUniqueSuffixTxt[nRand].wStringId);
			if (pString)
			{
				return pString;
			}
		}
	}

	return &sgptDataTables->wszDefault;
}

//D2Common.0x6FD68B60 (#10654)
wchar_t* __fastcall DATATBLS_RollRandomUniqueAppellationString(D2UnitStrc* pUnit)
{
	wchar_t* pString = NULL;
	int nRand = 0;

	if (pUnit && sgptDataTables->nUniqueAppellationTxtRecordCount > 0)
	{
		if ((sgptDataTables->nUniqueAppellationTxtRecordCount - 1) & sgptDataTables->nUniqueAppellationTxtRecordCount)
		{
			nRand = (unsigned int)SEED_RollRandomNumber(&pUnit->pSeed) % sgptDataTables->nUniqueAppellationTxtRecordCount;
		}
		else
		{
			nRand = SEED_RollRandomNumber(&pUnit->pSeed) & (sgptDataTables->nUniqueAppellationTxtRecordCount - 1);
		}

		if (sgptDataTables->pUniqueAppellationTxt[nRand].wStringId)
		{
			pString = D2LANG_GetStringFromTblIndex(sgptDataTables->pUniqueAppellationTxt[nRand].wStringId);
			if (pString)
			{
				return pString;
			}
		}
	}

	return &sgptDataTables->wszDefault;
}

//D2Common.0x6FD68BD0
void __fastcall DATATBLS_ReallocTCExInfo(D2TCExShortStrc* pTCExShort, int nNewRecordCount)
{
	pTCExShort->pInfo = (D2TCExInfoStrc*)D2_REALLOC_POOL(nullptr, pTCExShort->pInfo, sizeof(D2TCExInfoStrc) * nNewRecordCount);
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
		if (nValue1 <= 0x100000)
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
	int nId = FOG_GetRowFromTxt(sgptDataTables->pTreasureClassExLinker, szText, 0);
	if (nId >= 0)
	{
		return &sgptDataTables->pTreasureClassEx[nId];
	}

	return NULL;
}

//D2Common.0x6FD68DF0 (#10659)
D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromIdAndLevel(uint16_t wTCId, int nLvl)
{
	D2TCExShortStrc* pTCExRecord = NULL;
	D2TCExShortStrc* pNext = NULL;

	if (wTCId && wTCId < sgptDataTables->nTreasureClassEx)
	{
		pTCExRecord = &sgptDataTables->pTreasureClassEx[wTCId];
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
		return sgptDataTables->pChestTreasureClasses[nIndex + 3 * (nAct + 5 * nDifficulty)];
	}
	else
	{
		return sgptDataTables->pChestTreasureClasses[3 * (nAct + 5 * nDifficulty)];
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
			FOG_Trace("Couldn't parse treasure class modifier!  (Line:%d  Item:%d)\n", nCounter, nItemCounter);
			break;
		}

		*szArg = 0;
		++szArg;
		if (*szArg == 0)
		{
			FOG_Trace("Couldn't parse treasure class modifier!  (Line:%d  Item:%d)\n", nCounter, nItemCounter);
			break;
		}

		// NOLINTBEGIN(bugprone-branch-clone)
		szTmp = szNext;
		if (!strncmp(szTmp, "mul", 3))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nTxtRow = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cu", 2))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nUnique = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cs", 2))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nSet = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cr", 2))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nRare = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cm", 2))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nMagic = atoi(szArg);
		}
		else if (!strncmp(szTmp, "ce", 2))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nSuperior = atoi(szArg);
		}
		else if (!strncmp(szTmp, "cg", 2))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nNormal = atoi(szArg);
		}
		else if (!strncmp(szTmp, "ma", 2))
		{
			szNext = strchr(szArg, ',');
			if (szNext)
			{
				*szNext = 0;
				++szNext;
			}
			pTCExInfo->nTxtRow = atoi(szArg);
		}
		else if (!strncmp(szTmp, "mg", 2))
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
			FOG_Trace("Couldn't parse treasure class modifier!  (Line:%d  Item:%d)\n", nCounter, nItemCounter);
			break;
		}
		// NOLINTEND(bugprone-branch-clone)
	}
}

//1.10f: Inlined
//1.13c: D2Common.0x6FDA3780
D2TCExShortStrc* DATATBLS_NewTreasureClassEx(const char* nTreasureClass)
{
	FOG_10216_AddRecordToLinkingTable(sgptDataTables->pTreasureClassExLinker, nTreasureClass);

	if (!(sgptDataTables->nTreasureClassEx % 16))
	{
		sgptDataTables->pTreasureClassEx = (D2TCExShortStrc*)D2_REALLOC_POOL(NULL, sgptDataTables->pTreasureClassEx, sizeof(D2TCExShortStrc) * (sgptDataTables->nTreasureClassEx + 16));
	}
	memset(&sgptDataTables->pTreasureClassEx[sgptDataTables->nTreasureClassEx], 0x00, sizeof(sgptDataTables->pTreasureClassEx[sgptDataTables->nTreasureClassEx]));

	return &sgptDataTables->pTreasureClassEx[sgptDataTables->nTreasureClassEx];
}

//1.10f: Inlined
//1.13c: D2Common.0x6FDA4430
void DATATBLS_ParseTreasureClassItem(D2TCExShortStrc* pTCExTxtRecord, D2TreasureClassExTxt* pTreasureClassExTxt, int nLine, int nItem)
{
	const int32_t nCurrentItemProb = pTreasureClassExTxt[nLine].nProb[nItem];
	if (nCurrentItemProb <= 0)
	{
		FOG_Trace("Found a treasure class item with 0 chance!  (Line:%d  Item:%d)\n", nLine, nItem);
	}
	else
	{
		char* szText = pTreasureClassExTxt[nLine].szItem[nItem];
		if (szText[0] == '"')
		{
			++szText;
		}

		char* szTmp = szText;
		if (szTmp[0])
		{
			do
			{
				if (szTmp[0] == '"')
				{
					szTmp[0] = 0;
				}
				++szTmp;
			} while (szTmp[0]);
		}

		char* szNext = strchr(szText, ',');
		if (szNext)
		{
			szNext[0] = 0;
			++szNext;
		}

		// Try to find in normal items
		if (strlen(szText) <= 4)
		{
			int nItemId = 0;
			if (D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(DATATBLS_StringToCode(szText), &nItemId))
			{
				const int nNewTypeIdx = pTCExTxtRecord->nTypes;
				DATATBLS_ReallocTCExInfo(pTCExTxtRecord, nNewTypeIdx + 1);
				D2TCExInfoStrc* pTCExInfo = &pTCExTxtRecord->pInfo[nNewTypeIdx];

				pTCExInfo->nClassic = pTCExTxtRecord->nClassic;
				pTCExInfo->nProb = pTCExTxtRecord->nProb;
				pTCExInfo->nItemId = nItemId;
				if (pItemsTxtRecord->wVersion >= 100)
				{
					pTCExInfo->nFlags |= 0x10;
				}

				DATATBLS_UpdateTreasureClassProbabilities(pTCExTxtRecord, pTCExInfo, nCurrentItemProb, pItemsTxtRecord->wVersion >= 100);
				DATATBLS_ProcessAdditionalTreasureClassArguments(pTCExInfo, szNext, nLine, nItem);
				return;
			}
		}

		// Then try to find in treasure class
		{
			const int nTxtRow = FOG_GetRowFromTxt(sgptDataTables->pTreasureClassExLinker, szText, 0);
			if (nTxtRow > 0)
			{
				const int nNewTypeIdx = pTCExTxtRecord->nTypes;
				DATATBLS_ReallocTCExInfo(pTCExTxtRecord, nNewTypeIdx + 1);
				D2TCExInfoStrc* pTCExInfo = &pTCExTxtRecord->pInfo[nNewTypeIdx];

				pTCExInfo->nClassic = pTCExTxtRecord->nClassic;
				pTCExInfo->nProb = pTCExTxtRecord->nProb;
				pTCExInfo->nItemId = nTxtRow;
				pTCExInfo->nFlags |= 4;
				if (!sgptDataTables->pTreasureClassEx[nTxtRow].nClassic)
				{
					pTCExInfo->nFlags |= 0x10;
				}

				DATATBLS_UpdateTreasureClassProbabilities(pTCExTxtRecord, pTCExInfo, nCurrentItemProb, sgptDataTables->pTreasureClassEx[nTxtRow].nClassic == 0);
				DATATBLS_ProcessAdditionalTreasureClassArguments(pTCExInfo, szNext, nLine, nItem);
				return;
			}
		}

		// Then try to find in uniques
		if (sgptDataTables->pUniqueItemsLinker)
		{
			const int nTxtRow = FOG_GetRowFromTxt(sgptDataTables->pUniqueItemsLinker, szText, 0);
			if (nTxtRow > 0)
			{
				const int nNewTypeIdx = pTCExTxtRecord->nTypes;
				DATATBLS_ReallocTCExInfo(pTCExTxtRecord, nNewTypeIdx + 1);
				D2TCExInfoStrc* pTCExInfo = &pTCExTxtRecord->pInfo[nNewTypeIdx];

				pTCExInfo->nClassic = pTCExTxtRecord->nClassic;
				pTCExInfo->nProb = pTCExTxtRecord->nProb;
				int nItemId = 0;
				DATATBLS_GetItemRecordFromItemCode(sgptDataTables->pUniqueItemsTxt[nTxtRow].dwBaseItemCode, &nItemId);
				pTCExInfo->nFlags |= 0x11;
				pTCExInfo->nItemId = nItemId;
				pTCExInfo->nTxtRow = nTxtRow;

				DATATBLS_UpdateTreasureClassProbabilities(pTCExTxtRecord, pTCExInfo, nCurrentItemProb, TRUE);
				DATATBLS_ProcessAdditionalTreasureClassArguments(pTCExInfo, szNext, nLine, nItem);
				return;
			}
		}

		// Then try to find in sets
		if (sgptDataTables->pSetItemsLinker)
		{
			const int nTxtRow = FOG_GetRowFromTxt(sgptDataTables->pSetItemsLinker, szText, 0);
			if (nTxtRow >= 0)
			{
				const int nNewTypeIdx = pTCExTxtRecord->nTypes;
				DATATBLS_ReallocTCExInfo(pTCExTxtRecord, nNewTypeIdx + 1);
				D2TCExInfoStrc* pTCExInfo = &pTCExTxtRecord->pInfo[nNewTypeIdx];

				pTCExInfo->nClassic = pTCExTxtRecord->nClassic;
				pTCExInfo->nProb = pTCExTxtRecord->nProb;
				int nItemId = 0;
				DATATBLS_GetItemRecordFromItemCode(sgptDataTables->pSetItemsTxt[nTxtRow].szItemCode, &nItemId);
				pTCExInfo->nFlags |= 0x12;
				pTCExInfo->nItemId = nItemId;
				pTCExInfo->nTxtRow = nTxtRow;

				DATATBLS_UpdateTreasureClassProbabilities(pTCExTxtRecord, pTCExInfo, nCurrentItemProb, TRUE);
				DATATBLS_ProcessAdditionalTreasureClassArguments(pTCExInfo, szNext, nLine, nItem);
				return;
			}
		}
		else
		{
			FOG_Trace("Couldn't parse treasure class item!  (Line:%d  Item:%d)\n", nLine, nItem);
		}
	}
}

//D2Common.0x6FD68EC0
void __fastcall DATATBLS_LoadTreasureClassExTxt(void* pMemPool)
{
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
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	int nRecordCount = 0;
	D2TreasureClassExTxt* pTreasureClassExTxt = (D2TreasureClassExTxt*)DATATBLS_CompileTxt(pMemPool, "treasureclassex", pTbl, &nRecordCount, sizeof(D2TreasureClassExTxt));

	for (int i = 0; i < nRecordCount; ++i)
	{
		if (!pTreasureClassExTxt[i].szTreasureClass[0])
		{
			break;
		}

		D2TCExShortStrc* pTCExTxtRecord = DATATBLS_NewTreasureClassEx(pTreasureClassExTxt[i].szTreasureClass);
		++sgptDataTables->nTreasureClassEx;
		if (pTCExTxtRecord)
		{
			if (pTreasureClassExTxt[i].nGroup != 0)
			{
				pTCExTxtRecord->nGroup = sgptDataTables->nTreasureClassItemTypes + pTreasureClassExTxt[i].nGroup;
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

			for (int nItem = 0; nItem < 10; ++nItem)
			{
				if (!pTreasureClassExTxt[i].szItem[nItem][0])
				{
					break;
				}

				DATATBLS_ParseTreasureClassItem(pTCExTxtRecord, pTreasureClassExTxt, i, nItem);
			}
		}
	}

	DATATBLS_UnloadBin(pTreasureClassExTxt);
}

//D2Common.0x6FD69B70 (#10656)
int __stdcall DATATBLS_ShouldNeverCallInExpansion()
{
	FOG_DisplayWarning("should never call this function in expansion", __FILE__, __LINE__);
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
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMonItemPercentDataTables.pMonItemPercentTxt = (D2MonItemPercentTxt*)DATATBLS_CompileTxt(pMemPool, "monitempercent", pTbl, &sgptDataTables->pMonItemPercentDataTables.nMonItemPercentTxtRecordCount, sizeof(D2MonItemPercentTxt));
}

//D2Common.0x6FD69C40
void __fastcall DATATBLS_UnloadMonItemPercentTxt()
{
	DATATBLS_UnloadBin(sgptDataTables->pMonItemPercentDataTables.pMonItemPercentTxt);
	sgptDataTables->pMonItemPercentDataTables.pMonItemPercentTxt = NULL;
}

//D2Common.0x6FD69C50 (#10662)
D2MonItemPercentDataTbl* __fastcall DATATBLS_GetMonItemPercentDataTables()
{
	return &sgptDataTables->pMonItemPercentDataTables;
}

//D2Common.0x6FD69C60 (#10663)
D2MonItemPercentTxt* __stdcall DATATBLS_GetMonItemPercentTxtRecord(int nId)
{
	if (nId >= 0 && nId < sgptDataTables->pMonItemPercentDataTables.nMonItemPercentTxtRecordCount)
	{
		D2_ASSERT(sgptDataTables->pMonItemPercentDataTables.pMonItemPercentTxt);
		D2_ASSERT(&sgptDataTables->pMonItemPercentDataTables.pMonItemPercentTxt[nId]);
		return &sgptDataTables->pMonItemPercentDataTables.pMonItemPercentTxt[nId];
	}

	return NULL;
}

//D2Common.0x6FD69CD0
void __fastcall DATATBLS_LoadMonUModTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "uniquemod", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pMonUModLinker },
		{ "version", TXTFIELD_WORD, 0, 4, NULL },
		{ "enabled", TXTFIELD_BYTE, 0, 6, NULL },
		{ "xfer", TXTFIELD_BYTE, 0, 7, NULL },
		{ "champion", TXTFIELD_BYTE, 0, 8, NULL },
		{ "fpick", TXTFIELD_BYTE, 0, 9, NULL },
		{ "exclude1", TXTFIELD_NAMETOWORD, 0, 10, &sgptDataTables->pMonTypeLinker },
		{ "exclude2", TXTFIELD_NAMETOWORD, 0, 12, &sgptDataTables->pMonTypeLinker },
		{ "cpick", TXTFIELD_WORD, 0, 14, NULL },
		{ "cpick (N)", TXTFIELD_WORD, 0, 16, NULL },
		{ "cpick (H)", TXTFIELD_WORD, 0, 18, NULL },
		{ "upick", TXTFIELD_WORD, 0, 20, NULL },
		{ "upick (N)", TXTFIELD_WORD, 0, 22, NULL },
		{ "upick (H)", TXTFIELD_WORD, 0, 24, NULL },
		{ "constants", TXTFIELD_DWORD, 0, 28, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMonUModLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pMonUModTxt = (D2MonUModTxt*)DATATBLS_CompileTxt(pMemPool, "monumod", pTbl, &sgptDataTables->nMonUModTxtRecordCount, sizeof(D2MonUModTxt));

	if (sgptDataTables->nMonUModTxtRecordCount > 256)
	{
		FOG_Trace("monumod.txt exceeded %d entries", 256);
		sgptDataTables->nMonUModTxtRecordCount = 256;
	}
}

//D2Common.0x6FD69FA0
void __fastcall DATATBLS_LoadSuperUniquesTxt(void* pMemPool)
{
	int nId = 0;
	D2BinFieldStrc pTbl[] =
	{
		{ "Superunique", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pSuperUniquesLinker },
		{ "Name", TXTFIELD_KEYTOWORD, 0, TXTFIELD_DWORD, DATATBLS_GetStringIdFromReferenceString },
		{ "Class", TXTFIELD_NAMETODWORD, 0, 4, &sgptDataTables->pMonStatsLinker },
		{ "hcIdx", TXTFIELD_DWORD, 0, 8, NULL },
		{ "MonSound", TXTFIELD_NAMETODWORD, 0, 24, &sgptDataTables->pMonSoundsLinker },
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
		{ "TC", TXTFIELD_NAMETOWORD, 0, 44, &sgptDataTables->pTreasureClassExLinker },
		{ "TC(N)", TXTFIELD_NAMETOWORD, 0, 46, &sgptDataTables->pTreasureClassExLinker },
		{ "TC(H)", TXTFIELD_NAMETOWORD, 0, 48, &sgptDataTables->pTreasureClassExLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pSuperUniquesLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pSuperUniquesTxt = (D2SuperUniquesTxt*)DATATBLS_CompileTxt(pMemPool, "superuniques", pTbl, &sgptDataTables->nSuperUniquesTxtRecordCount, sizeof(D2SuperUniquesTxt));

	if (sgptDataTables->nSuperUniquesTxtRecordCount >= 512)
	{
		if (sgptDataTables->bCompileTxt)
		{
			FOG_Trace("Cut off superuniques at %d entries", 512);
		}
		sgptDataTables->nSuperUniquesTxtRecordCount = 512;
	}

	memset(sgptDataTables->nSuperUniqueIds, -1, sizeof(sgptDataTables->nSuperUniqueIds));

	for (int i = 0; i < sgptDataTables->nSuperUniquesTxtRecordCount; ++i)
	{
		nId = sgptDataTables->pSuperUniquesTxt[i].dwHcIdx;
		if (nId < 0 || nId >= ARRAY_SIZE(sgptDataTables->nSuperUniqueIds))
		{
			if (sgptDataTables->bCompileTxt)
			{
				FOG_Trace("Invalid hcIdx (%d) -- value must be between 0 and %d.", nId, 65);
			}
		}
		else
		{
			if (sgptDataTables->nSuperUniqueIds[nId] == -1)
			{
				sgptDataTables->nSuperUniqueIds[nId] = i;
			}
			else
			{
				if (sgptDataTables->bCompileTxt)
				{
					FOG_Trace("Duplicate hcIdx (%d) -- index must be unique.", nId);
				}
			}
		}
	}

	for (int i = 0; i < ARRAY_SIZE(sgptDataTables->nSuperUniqueIds); ++i)
	{
		if (sgptDataTables->nSuperUniqueIds[i] == -1 && sgptDataTables->bCompileTxt)
		{
			FOG_Trace("Missing hcIdx (%d) -- index incomplete", i);
		}

		const int SUPERUNIQUE_NONE = -1;
		D2_ASSERT(sgptDataTables->nSuperUniqueIds[i] != SUPERUNIQUE_NONE);
	}
}

//D2Common.0x6FD6A440 (#10668)
D2SuperUniquesTxt* __stdcall DATATBLS_GetSuperUniquesTxtRecord(int nSuperUniqueId)
{
	if (nSuperUniqueId >= 0 && nSuperUniqueId < sgptDataTables->nSuperUniquesTxtRecordCount)
	{
		return &sgptDataTables->pSuperUniquesTxt[nSuperUniqueId];
	}

	return NULL;
}

//D2Common.0x6FD6A470 (#11257)
int __fastcall DATATBLS_GetSuperUniquesTxtRecordCount()
{
	return sgptDataTables->nSuperUniquesTxtRecordCount;
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
		{ "hiredesc", TXTFIELD_CODETOBYTE, 0, 210, &sgptDataTables->pHireDescLinker },
		{ "defaultchance", TXTFIELD_DWORD, 0, 100, NULL },
		{ "skill1", TXTFIELD_NAMETODWORD, 0, 120, &sgptDataTables->pSkillsLinker },
		{ "mode1", TXTFIELD_BYTE, 0, 192, NULL },
		{ "chance1", TXTFIELD_DWORD, 0, 144, NULL },
		{ "chanceperlvl1", TXTFIELD_DWORD, 0, 168, NULL },
		{ "level1", TXTFIELD_BYTE, 0, 198, NULL },
		{ "lvlperlvl1", TXTFIELD_BYTE, 0, 204, NULL },
		{ "skill2", TXTFIELD_NAMETODWORD, 0, 124, &sgptDataTables->pSkillsLinker },
		{ "mode2", TXTFIELD_BYTE, 0, 193, NULL },
		{ "chance2", TXTFIELD_DWORD, 0, 148, NULL },
		{ "chanceperlvl2", TXTFIELD_DWORD, 0, 172, NULL },
		{ "level2", TXTFIELD_BYTE, 0, 199, NULL },
		{ "lvlperlvl2", TXTFIELD_BYTE, 0, 205, NULL },
		{ "skill3", TXTFIELD_NAMETODWORD, 0, 128, &sgptDataTables->pSkillsLinker },
		{ "mode3", TXTFIELD_BYTE, 0, 194, NULL },
		{ "chance3", TXTFIELD_DWORD, 0, 152, NULL },
		{ "chanceperlvl3", TXTFIELD_DWORD, 0, 176, NULL },
		{ "level3", TXTFIELD_BYTE, 0, 200, NULL },
		{ "lvlperlvl3", TXTFIELD_BYTE, 0, 206, NULL },
		{ "skill4", TXTFIELD_NAMETODWORD, 0, 132, &sgptDataTables->pSkillsLinker },
		{ "mode4", TXTFIELD_BYTE, 0, 195, NULL },
		{ "chance4", TXTFIELD_DWORD, 0, 156, NULL },
		{ "chanceperlvl4", TXTFIELD_DWORD, 0, 180, NULL },
		{ "level4", TXTFIELD_BYTE, 0, 201, NULL },
		{ "lvlperlvl4", TXTFIELD_BYTE, 0, 207, NULL },
		{ "skill5", TXTFIELD_NAMETODWORD, 0, 136, &sgptDataTables->pSkillsLinker },
		{ "mode5", TXTFIELD_BYTE, 0, 196, NULL },
		{ "chance5", TXTFIELD_DWORD, 0, 160, NULL },
		{ "chanceperlvl5", TXTFIELD_DWORD, 0, 184, NULL },
		{ "level5", TXTFIELD_BYTE, 0, 202, NULL },
		{ "lvlperlvl5", TXTFIELD_BYTE, 0, 208, NULL },
		{ "skill6", TXTFIELD_NAMETODWORD, 0, 140, &sgptDataTables->pSkillsLinker },
		{ "mode6", TXTFIELD_BYTE, 0, 197, NULL },
		{ "chance6", TXTFIELD_DWORD, 0, 164, NULL },
		{ "chanceperlvl6", TXTFIELD_DWORD, 0, 188, NULL },
		{ "level6", TXTFIELD_BYTE, 0, 203, NULL },
		{ "lvlperlvl6", TXTFIELD_BYTE, 0, 209, NULL },
		{ "head", TXTFIELD_DWORD, 0, 104, NULL },
		{ "torso", TXTFIELD_DWORD, 0, 108, NULL },
		{ "weapon", TXTFIELD_DWORD, 0, 112, NULL },
		{ "shield", TXTFIELD_DWORD, 0, 116, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pHirelingTxt = (D2HirelingTxt*)DATATBLS_CompileTxt(pMemPool, "hireling", pTbl, &sgptDataTables->nHirelingTxtRecordCount, sizeof(D2HirelingTxt));

	for (int i = 0; i < 256; ++i)
	{
		sgptDataTables->nClassicHirelingStartRecordIds[i] = -1;
		sgptDataTables->nExpansionHirelingStartRecordIds[i] = -1;
	}

	for (int i = 0; i < sgptDataTables->nHirelingTxtRecordCount; ++i)
	{
		sgptDataTables->pHirelingTxt[i].wNameFirst = D2LANG_GetTblIndex(sgptDataTables->pHirelingTxt[i].szNameFirst, &pUnicode);
		sgptDataTables->pHirelingTxt[i].wNameLast = D2LANG_GetTblIndex(sgptDataTables->pHirelingTxt[i].szNameLast, &pUnicode);

		if (FOG_IsExpansion())
		{
			D2_ASSERT(sgptDataTables->pHirelingTxt[i].wNameFirst > 0);
			D2_ASSERT(sgptDataTables->pHirelingTxt[i].wNameLast > sgptDataTables->pHirelingTxt[i].wNameFirst);

#define MAX_HIRELING_ID 256
			D2_ASSERT(sgptDataTables->pHirelingTxt[i].nId < MAX_HIRELING_ID);
		}

		if (sgptDataTables->pHirelingTxt[i].nId < 256)
		{
			nId = sgptDataTables->pHirelingTxt[i].nId + ((sgptDataTables->pHirelingTxt[i].wVersion >= 100) << 8);
			if (sgptDataTables->nClassicHirelingStartRecordIds[nId] < 0)
			{
				sgptDataTables->nClassicHirelingStartRecordIds[nId] = i;
			}
		}
	}
}

//D2Common.0x6FD6B1A0
D2ItemTypesTxt* __fastcall DATATBLS_GetItemTypesTxtRecord(int nItemType)
{
	if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
	{
		return &sgptDataTables->pItemTypesTxt[nItemType];
	}

	return NULL;
}

//D2Common.0x6FD6B1D0 (#10583)
D2HirelingTxt* __stdcall DATATBLS_GetHirelingTxtRecordFromIdAndLevel(BOOL bExpansion, int nId, int nLevel)
{
	if (nId >= 256)
	{
		return NULL;
	}

	const int nVersion = bExpansion != 0 ? 100 : 0;

	int nStartRecordId = sgptDataTables->nClassicHirelingStartRecordIds[nId + (bExpansion != FALSE) * 256];
	if (nStartRecordId < 0)
	{
		return nullptr;
	}
	D2HirelingTxt* pBestLevelHireling = nullptr;
	for(int nRecordIdx = nStartRecordId; nRecordIdx < sgptDataTables->nHirelingTxtRecordCount; nRecordIdx++)
	{
		D2HirelingTxt* pHirelingTxtRecord = &sgptDataTables->pHirelingTxt[nRecordIdx];

		if (pHirelingTxtRecord->nId == nId && pHirelingTxtRecord->wVersion == nVersion)
		{
			if (pBestLevelHireling != nullptr && pHirelingTxtRecord->nHirelingLevel > nLevel)
			{
				// We had found an hireling, but the level of the new one is too high
				return pBestLevelHireling;
			}
			pBestLevelHireling = pHirelingTxtRecord;

		}
		else if(pHirelingTxtRecord->nId > nId)
		{
			return pBestLevelHireling;
		}
	}

	return pBestLevelHireling;
}

//D2Common.0x6FD6B270 (#10585)
D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromNameId(BOOL bExpansion, uint16_t nNameId, D2HirelingTxt* pOldRecord)
{
	D2HirelingTxt* pHirelingTxtRecord = NULL;
	int nRecordId = 0;
	int nVersion = 0;

	nVersion = bExpansion != 0 ? 100 : 0;

	if (pOldRecord)
	{
		nRecordId = pOldRecord - sgptDataTables->pHirelingTxt + 1;
	}

	while (nRecordId < sgptDataTables->nHirelingTxtRecordCount)
	{
		pHirelingTxtRecord = &sgptDataTables->pHirelingTxt[nRecordId];

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
		nRecordId = pOldRecord - sgptDataTables->pHirelingTxt + 1;
	}

	while (nRecordId < sgptDataTables->nHirelingTxtRecordCount)
	{
		pHirelingTxtRecord = &sgptDataTables->pHirelingTxt[nRecordId];

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
		nRecordId = pOldRecord - sgptDataTables->pHirelingTxt + 1;
	}

	while (nRecordId < sgptDataTables->nHirelingTxtRecordCount)
	{
		pHirelingTxtRecord = &sgptDataTables->pHirelingTxt[nRecordId];

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
	uint32_t nLevel = 0;

	nVersion = bExpansion != 0 ? 100 : 0;

	if (pOldRecord)
	{
		nLevel = pOldRecord->nHirelingLevel;
		nRecordId = pOldRecord - sgptDataTables->pHirelingTxt + 1;
	}

	while (nRecordId < sgptDataTables->nHirelingTxtRecordCount)
	{
		pHirelingTxtRecord = &sgptDataTables->pHirelingTxt[nRecordId];

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
		{ "npc", TXTFIELD_NAMETODWORD, 0, 0, &sgptDataTables->pMonStatsLinker },
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
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pNpcTxt = (D2NpcTxt*)DATATBLS_CompileTxt(pMemPool, "npc", pTbl, &sgptDataTables->nNpcTxtRecordCount, sizeof(D2NpcTxt));
}

//D2Common.0x6FD6B820 (#10588)
D2NpcTxt* __stdcall DATATBLS_GetNpcTxtRecord(uint32_t dwNpcId)
{
	for (int i = 0; i < sgptDataTables->nNpcTxtRecordCount; ++i)
	{
		if (sgptDataTables->pNpcTxt[i].dwNpc == dwNpcId)
		{
			return &sgptDataTables->pNpcTxt[i];
		}
	}

	return NULL;
}

//D2Common.0x6FD6B850
void __fastcall DATATBLS_LoadMonSoundsTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Id", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pMonSoundsLinker },
		{ "Attack1", TXTFIELD_NAMETODWORD, 0, 4, &sgptDataTables->pSoundsLinker },
		{ "Att1Del", TXTFIELD_DWORD, 0, 8, NULL },
		{ "Att1Prb", TXTFIELD_DWORD, 0, 12, NULL },
		{ "Attack2", TXTFIELD_NAMETODWORD, 0, 28, &sgptDataTables->pSoundsLinker },
		{ "Att2Del", TXTFIELD_DWORD, 0, 32, NULL },
		{ "Att2Prb", TXTFIELD_DWORD, 0, 36, NULL },
		{ "Weapon1", TXTFIELD_NAMETODWORD, 0, 16, &sgptDataTables->pSoundsLinker },
		{ "Wea1Del", TXTFIELD_DWORD, 0, 20, NULL },
		{ "Wea1Vol", TXTFIELD_DWORD, 0, 24, NULL },
		{ "Weapon2", TXTFIELD_NAMETODWORD, 0, 40, &sgptDataTables->pSoundsLinker },
		{ "Wea2Del", TXTFIELD_DWORD, 0, 44, NULL },
		{ "Wea2Vol", TXTFIELD_DWORD, 0, 48, NULL },
		{ "HitSound", TXTFIELD_NAMETODWORD, 0, 52, &sgptDataTables->pSoundsLinker },
		{ "HitDelay", TXTFIELD_DWORD, 0, 56, NULL },
		{ "DeathSound", TXTFIELD_NAMETODWORD, 0, 60, &sgptDataTables->pSoundsLinker },
		{ "DeaDelay", TXTFIELD_DWORD, 0, 64, NULL },
		{ "Skill1", TXTFIELD_NAMETODWORD, 0, 68, &sgptDataTables->pSoundsLinker },
		{ "Skill2", TXTFIELD_NAMETODWORD, 0, 72, &sgptDataTables->pSoundsLinker },
		{ "Skill3", TXTFIELD_NAMETODWORD, 0, 76, &sgptDataTables->pSoundsLinker },
		{ "Skill4", TXTFIELD_NAMETODWORD, 0, 80, &sgptDataTables->pSoundsLinker },
		{ "Footstep", TXTFIELD_NAMETODWORD, 0, 84, &sgptDataTables->pSoundsLinker },
		{ "FootstepLayer", TXTFIELD_NAMETODWORD, 0, 88, &sgptDataTables->pSoundsLinker },
		{ "FsCnt", TXTFIELD_DWORD, 0, 92, NULL },
		{ "FsOff", TXTFIELD_DWORD, 0, 96, NULL },
		{ "FsPrb", TXTFIELD_DWORD, 0, 100, NULL },
		{ "Neutral", TXTFIELD_NAMETODWORD, 0, 104, &sgptDataTables->pSoundsLinker },
		{ "NeuTime", TXTFIELD_DWORD, 0, 108, NULL },
		{ "Init", TXTFIELD_NAMETODWORD, 0, 112, &sgptDataTables->pSoundsLinker },
		{ "Taunt", TXTFIELD_NAMETODWORD, 0, 116, &sgptDataTables->pSoundsLinker },
		{ "Flee", TXTFIELD_NAMETODWORD, 0, 120, &sgptDataTables->pSoundsLinker },
		{ "CvtMo1", TXTFIELD_CODETOBYTE, 0, 124, &sgptDataTables->pMonModeLinker },
		{ "CvtMo2", TXTFIELD_CODETOBYTE, 0, 132, &sgptDataTables->pMonModeLinker },
		{ "CvtMo3", TXTFIELD_CODETOBYTE, 0, 140, &sgptDataTables->pMonModeLinker },
		{ "CvtSk1", TXTFIELD_NAMETODWORD, 0, 128, &sgptDataTables->pSkillsLinker },
		{ "CvtSk2", TXTFIELD_NAMETODWORD, 0, 136, &sgptDataTables->pSkillsLinker },
		{ "CvtSk3", TXTFIELD_NAMETODWORD, 0, 144, &sgptDataTables->pSkillsLinker },
		{ "CvtTgt1", TXTFIELD_CODETOBYTE, 0, 125, &sgptDataTables->pMonModeLinker },
		{ "CvtTgt2", TXTFIELD_CODETOBYTE, 0, 133, &sgptDataTables->pMonModeLinker },
		{ "CvtTgt3", TXTFIELD_CODETOBYTE, 0, 141, &sgptDataTables->pMonModeLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMonSoundsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pMonSoundsTxt = (D2MonSoundsTxt*)DATATBLS_CompileTxt(pMemPool, "monsounds", pTbl, &sgptDataTables->nMonSoundsTxtRecordCount, sizeof(D2MonSoundsTxt));
}

//D2Common.0x6FD6BF50 (#11252)
D2MonSoundsTxt* __stdcall DATATBLS_GetMonSoundsTxtRecordFromSoundId(int nSoundId)
{
	if (sgptDataTables->pMonSoundsTxt && nSoundId >= 0 && nSoundId < sgptDataTables->nMonSoundsTxtRecordCount)
	{
		return &sgptDataTables->pMonSoundsTxt[nSoundId];
	}

	return NULL;
}

//D2Common.0x6FD6BF80 (#11250)
D2MonSoundsTxt* __stdcall DATATBLS_GetMonSoundsTxtRecordFromMonsterId(int nMonsterId)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nSoundId = 0;

	pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
	if (sgptDataTables->pMonSoundsTxt && pMonStatsTxtRecord)
	{
		nSoundId = pMonStatsTxtRecord->wMonSound;
		if (nSoundId >= 0 && nSoundId < sgptDataTables->nMonSoundsTxtRecordCount)
		{
			return &sgptDataTables->pMonSoundsTxt[nSoundId];
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


				pMonStats2TxtRecord->unk0x26[nOffset].nComposit[nCounter] = FOG_GetLinkIndex(sgptDataTables->pCompCodeLinker, DATATBLS_StringToCode(pTmp), 1);

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
		{ "Id", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pMonStats2Linker },
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
		{ "ResurrectMode", TXTFIELD_CODETOBYTE, 0, 267, &sgptDataTables->pMonModeLinker },
		{ "ResurrectSkill", TXTFIELD_NAMETOWORD, 0, 268, &sgptDataTables->pSkillsLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMonStats2Linker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pMonStats2Txt = (D2MonStats2Txt*)DATATBLS_CompileTxt(pMemPool, "monstats2", pTbl, &sgptDataTables->nMonStats2TxtRecordCount, sizeof(D2MonStats2Txt));
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

	if (nMonType1 > 0 && nMonType1 < sgptDataTables->nMonTypeTxtRecordCount)
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

			if (nMonsterType >= sgptDataTables->nMonTypeTxtRecordCount)
			{
				break;
			}

			if (nIndex > 124)
			{
				return 0;
			}

			pMonTypeTxtRecord = &sgptDataTables->pMonTypeTxt[nMonsterType];

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

		FOG_Trace("Invalid monster type at line %d of file %s", __LINE__, __FILE__);
	}

	return 0;
}

//D2Common.0x6FD6D730
void __fastcall DATATBLS_LoadMonTypeTxt(void* pMemPool)
{
	uint32_t* pMonTypeNest = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "type", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pMonTypeLinker },
		{ "equiv1", TXTFIELD_NAMETOWORD, 0, 2, &sgptDataTables->pMonTypeLinker },
		{ "equiv2", TXTFIELD_NAMETOWORD, 0, 4, &sgptDataTables->pMonTypeLinker },
		{ "equiv3", TXTFIELD_NAMETOWORD, 0, 6, &sgptDataTables->pMonTypeLinker },
		{ "strsing", TXTFIELD_KEYTOWORD, 0, 8, DATATBLS_GetStringIdFromReferenceString },
		{ "strplur", TXTFIELD_KEYTOWORD, 0, 10, DATATBLS_GetStringIdFromReferenceString },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMonTypeLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pMonTypeTxt = (D2MonTypeTxt*)DATATBLS_CompileTxt(pMemPool, "montype", pTbl, &sgptDataTables->nMonTypeTxtRecordCount, sizeof(D2MonTypeTxt));

	sgptDataTables->nMonTypeIndex = (sgptDataTables->nMonTypeTxtRecordCount + 31) / 32;
	sgptDataTables->pMonTypeNest = (uint32_t*)D2_CALLOC_POOL(nullptr, sizeof(uint32_t) * sgptDataTables->nMonTypeTxtRecordCount * sgptDataTables->nMonTypeIndex);

	for (int i = 0; i < sgptDataTables->nMonTypeTxtRecordCount; ++i)
	{
		pMonTypeNest = &sgptDataTables->pMonTypeNest[sgptDataTables->nMonTypeIndex * i];

		for (int j = 0; j < sgptDataTables->nMonTypeTxtRecordCount; ++j)
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
	if (sgptDataTables->pMonTypeTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonTypeTxt);
		FOG_FreeLinker(sgptDataTables->pMonTypeLinker);
		D2_FREE_POOL(nullptr, sgptDataTables->pMonTypeNest);
		sgptDataTables->pMonTypeTxt = NULL;
		sgptDataTables->pMonTypeLinker = NULL;
	}
}

//D2Common.0x6FD6D960
void __fastcall DATATBLS_LoadMonPropTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Id", TXTFIELD_NAMETOINDEX2, 0, 0, &sgptDataTables->pMonPropLinker },
		{ "prop1", TXTFIELD_NAMETODWORD, 0, 4, &sgptDataTables->pPropertiesLinker },
		{ "par1", TXTFIELD_DWORD, 0, 8, NULL },
		{ "chance1", TXTFIELD_BYTE, 0, 292, NULL },
		{ "min1", TXTFIELD_DWORD, 0, 12, NULL },
		{ "max1", TXTFIELD_DWORD, 0, 16, NULL },
		{ "prop2", TXTFIELD_NAMETODWORD, 0, 20, &sgptDataTables->pPropertiesLinker },
		{ "chance2", TXTFIELD_BYTE, 0, 293, NULL },
		{ "par2", TXTFIELD_DWORD, 0, 24, NULL },
		{ "min2", TXTFIELD_DWORD, 0, 28, NULL },
		{ "max2", TXTFIELD_DWORD, 0, 32, NULL },
		{ "prop3", TXTFIELD_NAMETODWORD, 0, 36, &sgptDataTables->pPropertiesLinker },
		{ "chance3", TXTFIELD_BYTE, 0, 294, NULL },
		{ "par3", TXTFIELD_DWORD, 0, 40, NULL },
		{ "min3", TXTFIELD_DWORD, 0, 44, NULL },
		{ "max3", TXTFIELD_DWORD, 0, 48, NULL },
		{ "prop4", TXTFIELD_NAMETODWORD, 0, 52, &sgptDataTables->pPropertiesLinker },
		{ "chance4", TXTFIELD_BYTE, 0, 295, NULL },
		{ "par4", TXTFIELD_DWORD, 0, 56, NULL },
		{ "min4", TXTFIELD_DWORD, 0, 60, NULL },
		{ "max4", TXTFIELD_DWORD, 0, 64, NULL },
		{ "prop5", TXTFIELD_NAMETODWORD, 0, 68, &sgptDataTables->pPropertiesLinker },
		{ "chance5", TXTFIELD_BYTE, 0, 296, NULL },
		{ "par5", TXTFIELD_DWORD, 0, 72, NULL },
		{ "min5", TXTFIELD_DWORD, 0, 76, NULL },
		{ "max5", TXTFIELD_DWORD, 0, 80, NULL },
		{ "prop6", TXTFIELD_NAMETODWORD, 0, 84, &sgptDataTables->pPropertiesLinker },
		{ "chance6", TXTFIELD_BYTE, 0, 297, NULL },
		{ "par6", TXTFIELD_DWORD, 0, 88, NULL },
		{ "min6", TXTFIELD_DWORD, 0, 92, NULL },
		{ "max6", TXTFIELD_DWORD, 0, 96, NULL },
		{ "prop1 (N)", TXTFIELD_NAMETODWORD, 0, 100, &sgptDataTables->pPropertiesLinker },
		{ "chance1 (N)", TXTFIELD_BYTE, 0, 298, NULL },
		{ "par1 (N)", TXTFIELD_DWORD, 0, 104, NULL },
		{ "min1 (N)", TXTFIELD_DWORD, 0, 108, NULL },
		{ "max1 (N)", TXTFIELD_DWORD, 0, 112, NULL },
		{ "prop2 (N)", TXTFIELD_NAMETODWORD, 0, 116, &sgptDataTables->pPropertiesLinker },
		{ "chance2 (N)", TXTFIELD_BYTE, 0, 299, NULL },
		{ "par2 (N)", TXTFIELD_DWORD, 0, 120, NULL },
		{ "min2 (N)", TXTFIELD_DWORD, 0, 124, NULL },
		{ "max2 (N)", TXTFIELD_DWORD, 0, 128, NULL },
		{ "prop3 (N)", TXTFIELD_NAMETODWORD, 0, 132, &sgptDataTables->pPropertiesLinker },
		{ "chance3 (N)", TXTFIELD_BYTE, 0, 300, NULL },
		{ "par3 (N)", TXTFIELD_DWORD, 0, 136, NULL },
		{ "min3 (N)", TXTFIELD_DWORD, 0, 140, NULL },
		{ "max3 (N)", TXTFIELD_DWORD, 0, 144, NULL },
		{ "prop4 (N)", TXTFIELD_NAMETODWORD, 0, 148, &sgptDataTables->pPropertiesLinker },
		{ "chance4 (N)", TXTFIELD_BYTE, 0, 301, NULL },
		{ "par4 (N)", TXTFIELD_DWORD, 0, 152, NULL },
		{ "min4 (N)", TXTFIELD_DWORD, 0, 156, NULL },
		{ "max4 (N)", TXTFIELD_DWORD, 0, 160, NULL },
		{ "prop5 (N)", TXTFIELD_NAMETODWORD, 0, 164, &sgptDataTables->pPropertiesLinker },
		{ "chance5 (N)", TXTFIELD_BYTE, 0, 302, NULL },
		{ "par5 (N)", TXTFIELD_DWORD, 0, 168, NULL },
		{ "min5 (N)", TXTFIELD_DWORD, 0, 172, NULL },
		{ "max5 (N)", TXTFIELD_DWORD, 0, 176, NULL },
		{ "prop6 (N)", TXTFIELD_NAMETODWORD, 0, 180, &sgptDataTables->pPropertiesLinker },
		{ "chance6 (N)", TXTFIELD_BYTE, 0, 303, NULL },
		{ "par6 (N)", TXTFIELD_DWORD, 0, 184, NULL },
		{ "min6 (N)", TXTFIELD_DWORD, 0, 188, NULL },
		{ "max6 (N)", TXTFIELD_DWORD, 0, 192, NULL },
		{ "prop1 (H)", TXTFIELD_NAMETODWORD, 0, 196, &sgptDataTables->pPropertiesLinker },
		{ "chance1 (H)", TXTFIELD_BYTE, 0, 304, NULL },
		{ "par1 (H)", TXTFIELD_DWORD, 0, 200, NULL },
		{ "min1 (H)", TXTFIELD_DWORD, 0, 204, NULL },
		{ "max1 (H)", TXTFIELD_DWORD, 0, 208, NULL },
		{ "prop2 (H)", TXTFIELD_NAMETODWORD, 0, 212, &sgptDataTables->pPropertiesLinker },
		{ "chance2 (H)", TXTFIELD_BYTE, 0, 305, NULL },
		{ "par2 (H)", TXTFIELD_DWORD, 0, 216, NULL },
		{ "min2 (H)", TXTFIELD_DWORD, 0, 220, NULL },
		{ "max2 (H)", TXTFIELD_DWORD, 0, 224, NULL },
		{ "prop3 (H)", TXTFIELD_NAMETODWORD, 0, 228, &sgptDataTables->pPropertiesLinker },
		{ "chance3 (H)", TXTFIELD_BYTE, 0, 306, NULL },
		{ "par3 (H)", TXTFIELD_DWORD, 0, 232, NULL },
		{ "min3 (H)", TXTFIELD_DWORD, 0, 236, NULL },
		{ "max3 (H)", TXTFIELD_DWORD, 0, 240, NULL },
		{ "prop4 (H)", TXTFIELD_NAMETODWORD, 0, 244, &sgptDataTables->pPropertiesLinker },
		{ "chance4 (H)", TXTFIELD_BYTE, 0, 307, NULL },
		{ "par4 (H)", TXTFIELD_DWORD, 0, 248, NULL },
		{ "min4 (H)", TXTFIELD_DWORD, 0, 252, NULL },
		{ "max4 (H)", TXTFIELD_DWORD, 0, 256, NULL },
		{ "prop5 (H)", TXTFIELD_NAMETODWORD, 0, 260, &sgptDataTables->pPropertiesLinker },
		{ "chance5 (H)", TXTFIELD_BYTE, 0, 308, NULL },
		{ "par5 (H)", TXTFIELD_DWORD, 0, 264, NULL },
		{ "min5 (H)", TXTFIELD_DWORD, 0, 268, NULL },
		{ "max5 (H)", TXTFIELD_DWORD, 0, 272, NULL },
		{ "prop6 (H)", TXTFIELD_NAMETODWORD, 0, 276, &sgptDataTables->pPropertiesLinker },
		{ "chance6 (H)", TXTFIELD_BYTE, 0, 309, NULL },
		{ "par6 (H)", TXTFIELD_DWORD, 0, 280, NULL },
		{ "min6 (H)", TXTFIELD_DWORD, 0, 284, NULL },
		{ "max6 (H)", TXTFIELD_DWORD, 0, 288, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMonPropLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pMonPropTxt = (D2MonPropTxt*)DATATBLS_CompileTxt(pMemPool, "monprop", pTbl, &sgptDataTables->nMonPropTxtRecordCount, sizeof(D2MonPropTxt));
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
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMonLvlTxt = (D2MonLvlTxt*)DATATBLS_CompileTxt(pMemPool, "monlvl", pTbl, &sgptDataTables->nMonLvlTxtRecordCount, sizeof(D2MonLvlTxt));
}

//D2Common.0x6FD6EDE0
void __fastcall DATATBLS_MonPresetPlaceLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn)
{
	int nRow = 0;

	if (pRecord)
	{
		*((uint8_t*)pRecord + 1) = 0;
		*((uint16_t*)pRecord + 1) = 0;

		if (pSrc && *pSrc)
		{
			nRow = FOG_GetRowFromTxt(sgptDataTables->pSuperUniquesLinker, pSrc, 0);
			if (nRow < 0)
			{
				nRow = FOG_GetRowFromTxt(sgptDataTables->pMonStatsLinker, pSrc, 0);
				if (nRow < 0)
				{
					nRow = FOG_GetRowFromTxt(sgptDataTables->pMonPlaceLinker, pSrc, 1);
					if (nRow >= 0)
					{
						*((uint8_t*)pRecord + 1) = 0;
						*((uint16_t*)pRecord + 1) = nRow;
					}
				}
				else
				{
					*((uint8_t*)pRecord + 1) = 1;
					*((uint16_t*)pRecord + 1) = nRow;
				}
			}
			else
			{
				*((uint8_t*)pRecord + 1) = 2;
				*((uint16_t*)pRecord + 1) = nRow;
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
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	pMonPresetTxt = (D2MonPresetTxt*)DATATBLS_CompileTxt(pMemPool, "monpreset", pTbl, &nRecordCount, sizeof(D2MonPresetTxt));

	sgptDataTables->pMonPresetTxt = pMonPresetTxt;
	sgptDataTables->pMonPresetTxtActSections[0] = pMonPresetTxt;

	for (int i = 0; i < nRecordCount; ++i)
	{
		for (int j = nAct + 1; j < pMonPresetTxt[i].nAct; ++j)
		{
			sgptDataTables->nMonPresetTxtActRecordCounts[nAct] = nActRecords;
			++nAct;
			nActRecords = 0;
			sgptDataTables->pMonPresetTxtActSections[nAct] = &pMonPresetTxt[i];
		}

		++nActRecords;
	}

	sgptDataTables->nMonPresetTxtActRecordCounts[nAct] = nActRecords;
}

//D2Common.0x6FD6EF30 (#11256)
D2MonPresetTxt* __stdcall DATATBLS_GetMonPresetTxtActSection(int nAct, int* pRecordCount)
{
	if (pRecordCount)
	{
		if (nAct >= 0 && nAct < 5 && sgptDataTables->pMonPresetTxt)
		{
			*pRecordCount = sgptDataTables->nMonPresetTxtActRecordCounts[nAct];
			return sgptDataTables->pMonPresetTxtActSections[nAct];
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
		return nSuperUnique + sgptDataTables->nSuperUniquesTxtRecordCount + sgptDataTables->nMonStatsTxtRecordCount;

	case 2:
		return nSuperUnique + sgptDataTables->nMonStatsTxtRecordCount;

	default:
		return nSuperUnique;

	}
}

//D2Common.0x6FD6EFA0
void __fastcall DATATBLS_LoadSoundsTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Sound", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pSoundsLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	if (sgptDataTables->bCompileTxt)
	{
		sgptDataTables->pSoundsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pSoundsTxtCodes = (D2SoundsTxtStub*)DATATBLS_CompileTxt(pMemPool, "sounds", pTbl, &sgptDataTables->nSoundsTxtCodes, sizeof(D2SoundsTxtStub));
	}
}

//D2Common.0x6FD6F020
void __fastcall DATATBLS_UnloadSoundsTxt()
{
	if (sgptDataTables->pSoundsTxtCodes)
	{
		DATATBLS_UnloadBin(sgptDataTables->pSoundsTxtCodes);
		sgptDataTables->pSoundsTxtCodes = NULL;
		FOG_FreeLinker(sgptDataTables->pSoundsLinker);
		sgptDataTables->pSoundsLinker = NULL;
		sgptDataTables->nSoundsTxtCodes = 0;
	}
}


//D2Common.0x6FD6F230
void __fastcall DATATBLS_LoadMonEquipTxt(void* pMemPool)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2MonEquipTxt* pMonEquipTxtRecord = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "monster", TXTFIELD_NAMETOWORD, 0, 0, &sgptDataTables->pMonStatsLinker },
		{ "level", TXTFIELD_WORD, 0, 2, NULL },
		{ "oninit", TXTFIELD_BYTE, 0, 4, NULL },
		{ "item1", TXTFIELD_RAW, 0, 8, &sgptDataTables->pItemsLinker },
		{ "loc1", TXTFIELD_CODETOBYTE, 0, 20, &sgptDataTables->pBodyLocsLinker },
		{ "mod1", TXTFIELD_BYTE, 0, 23, NULL },
		{ "item2", TXTFIELD_RAW, 0, 12, &sgptDataTables->pItemsLinker },
		{ "loc2", TXTFIELD_CODETOBYTE, 0, 21, &sgptDataTables->pBodyLocsLinker },
		{ "mod2", TXTFIELD_BYTE, 0, 24, NULL },
		{ "item3", TXTFIELD_RAW, 0, 16, &sgptDataTables->pItemsLinker },
		{ "loc3", TXTFIELD_CODETOBYTE, 0, 22, &sgptDataTables->pBodyLocsLinker },
		{ "mod3", TXTFIELD_BYTE, 0, 25, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pMonEquipTxt = (D2MonEquipTxt*)DATATBLS_CompileTxt(pMemPool, "monequip", pTbl, &sgptDataTables->nMonEquipTxtRecordCount, sizeof(D2MonEquipTxt));

	if (sgptDataTables->nMonEquipTxtRecordCount >= 32767)
	{
		FOG_DisplayWarning("Monequip.txt exceeded maximum number of allowable rows", __FILE__, __LINE__);
	}

	for (int i = 0; i < sgptDataTables->nMonStatsTxtRecordCount; ++i)
	{
		sgptDataTables->pMonStatsTxt[i].nMonEquipTxtRecordId = -1;
	}

	for (int i = 0; i < sgptDataTables->nMonEquipTxtRecordCount; ++i)
	{
		pMonEquipTxtRecord = &sgptDataTables->pMonEquipTxt[i];
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
					|| pMonEquipTxtRecord->dwItem[j] != '    ' && FOG_GetLinkIndex(sgptDataTables->pItemsLinker, pMonEquipTxtRecord->dwItem[j], 1) < 0)
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
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pUniqueTitleTxt = (D2UniqueTitleTxt*)DATATBLS_CompileTxt(pMemPool, "uniquetitle", pTbl, &sgptDataTables->nUniqueTitleTxtRecordCount, sizeof(D2UniqueTitleTxt));
	sgptDataTables->pUniquePrefixTxt = (D2UniquePrefixTxt*)DATATBLS_CompileTxt(pMemPool, "uniqueprefix", pTbl, &sgptDataTables->nUniquePrefixTxtRecordCount, sizeof(D2UniquePrefixTxt));
	sgptDataTables->pUniqueSuffixTxt = (D2UniqueSuffixTxt*)DATATBLS_CompileTxt(pMemPool, "uniquesuffix", pTbl, &sgptDataTables->nUniqueSuffixTxtRecordCount, sizeof(D2UniqueSuffixTxt));
	sgptDataTables->pUniqueAppellationTxt = (D2UniqueAppellationTxt*)DATATBLS_CompileTxt(pMemPool, "uniqueappellation", pTbl, &sgptDataTables->nUniqueAppellationTxtRecordCount, sizeof(D2UniqueAppellationTxt));

	DATATBLS_LoadMonLvlTxt(pMemPool);

	sgptDataTables->pTreasureClassExLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);

	DATATBLS_CreateItemTypeTreasureClasses();
	DATATBLS_LoadTreasureClassExTxt(pMemPool);

	ppTCEx = sgptDataTables->pChestTreasureClasses;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				// Note: Game actually uses Storm.dll SStrVPrintf
				wsprintfA(szChest, "Act %d%s Chest %s", j + 1, szDifficulties[i], szTreasureClassVariants[k]);

				nTxtRow = FOG_GetRowFromTxt(sgptDataTables->pTreasureClassExLinker, szChest, 0);
				if (nTxtRow >= 0)
				{
					*ppTCEx = &sgptDataTables->pTreasureClassEx[nTxtRow];
				}
				else
				{
					*ppTCEx = 0;
				}

				++ppTCEx;
			}
		}
	}

	D2_ASSERT(sgptDataTables->nTreasureClassEx < USHRT_MAX);

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

	FOG_10216_AddRecordToLinkingTable(sgptDataTables->pTreasureClassExLinker, &sgptDataTables->szDefaultString);

	if (!(sgptDataTables->nTreasureClassEx % 16))
	{
		sgptDataTables->pTreasureClassEx = (D2TCExShortStrc*)D2_REALLOC_POOL(NULL, sgptDataTables->pTreasureClassEx, sizeof(D2TCExShortStrc) * (sgptDataTables->nTreasureClassEx + 16));
	}
	memset(&sgptDataTables->pTreasureClassEx[sgptDataTables->nTreasureClassEx], 0x00, sizeof(sgptDataTables->pTreasureClassEx[sgptDataTables->nTreasureClassEx]));
	++sgptDataTables->nTreasureClassEx;

	for (int i = 0; i < sgptDataTables->nItemTypesTxtRecordCount; ++i)
	{
		if (!&sgptDataTables->pItemTypesTxt[i])
		{
			break;
		}

		if (sgptDataTables->pItemTypesTxt[i].nTreasureClass)
		{
			++sgptDataTables->nTreasureClassItemTypes;

			nLevel = 3;
			do
			{
				if (FOG_GetStringFromLinkIndex(sgptDataTables->pItemTypesLinker, i, v37))
				{
					// Note: Game actually uses Storm.dll SStrVPrintf
					wsprintfA(dest, "%s%d", v37, nLevel);
					FOG_10216_AddRecordToLinkingTable(sgptDataTables->pTreasureClassExLinker, dest);

					if (!(sgptDataTables->nTreasureClassEx % 16))
					{
						sgptDataTables->pTreasureClassEx = (D2TCExShortStrc*)D2_REALLOC_POOL(NULL, sgptDataTables->pTreasureClassEx, sizeof(D2TCExShortStrc) * (sgptDataTables->nTreasureClassEx + 16));
					}
					memset(&sgptDataTables->pTreasureClassEx[sgptDataTables->nTreasureClassEx], 0x00, sizeof(sgptDataTables->pTreasureClassEx[sgptDataTables->nTreasureClassEx]));

					pTCExTxtRecord = &sgptDataTables->pTreasureClassEx[sgptDataTables->nTreasureClassEx++];

					if (pTCExTxtRecord)
					{
						pTCExTxtRecord->nPicks = 1;
						pTCExTxtRecord->nGroup = 0;
						pTCExTxtRecord->nLevel = nLevel - 3;

						for (int j = 0; j < sgptDataTables->pItemDataTables.nItemsTxtRecordCount; ++j)
						{
							pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(j);
							if (pItemsTxtRecord && !pItemsTxtRecord->nQuest && pItemsTxtRecord->nSpawnable && ITEMS_CheckItemTypeIdByItemId(j, i) && (i == ITEMTYPE_MISSILE_POTION || !ITEMS_CheckItemTypeIdByItemId(j, ITEMTYPE_MISSILE_POTION)))
							{
								if (pItemsTxtRecord->nLevel > nLevel - 3 && pItemsTxtRecord->nLevel <= nLevel)
								{
									nItemType = pItemsTxtRecord->wType[0];
									if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
									{
										pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
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
	if (sgptDataTables->pMonEquipTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonEquipTxt);
		sgptDataTables->nMonEquipTxtRecordCount = 0;
	}

	DATATBLS_UnloadBin(sgptDataTables->pNpcTxt);

	DATATBLS_UnloadBin(sgptDataTables->pHirelingTxt);
	sgptDataTables->nHirelingTxtRecordCount = 0;
	for (int i = 0; i < 256; ++i)
	{
		sgptDataTables->nClassicHirelingStartRecordIds[i] = -1;
		sgptDataTables->nExpansionHirelingStartRecordIds[i] = -1;
	}

	if (sgptDataTables->pMonPresetTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonPresetTxt);
	}
	for (int i = 0; i < 5; ++i)
	{
		sgptDataTables->pMonPresetTxtActSections[i] = NULL;
		sgptDataTables->nMonPresetTxtActRecordCounts[i] = 0;
	}

	if (sgptDataTables->pSuperUniquesLinker)
	{
		FOG_FreeLinker(sgptDataTables->pSuperUniquesLinker);
	}
	if (sgptDataTables->pSuperUniquesTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pSuperUniquesTxt);
		sgptDataTables->pSuperUniquesTxt = NULL;
	}
	sgptDataTables->nSuperUniquesTxtRecordCount = 0;

	if (sgptDataTables->pMonUModLinker)
	{
		FOG_FreeLinker(sgptDataTables->pMonUModLinker);
		sgptDataTables->pMonUModLinker = NULL;
	}
	if (sgptDataTables->pMonUModTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonUModTxt);
		sgptDataTables->pMonUModTxt = NULL;
	}
	sgptDataTables->nMonUModTxtRecordCount = 0;

	if (sgptDataTables->pMonStatsTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonStatsTxt);
		FOG_FreeLinker(sgptDataTables->pMonStatsLinker);
	}
	sgptDataTables->pMonStatsTxt = NULL;

	if (sgptDataTables->pMonStats2Txt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonStats2Txt);
		FOG_FreeLinker(sgptDataTables->pMonStats2Linker);
	}

	if (sgptDataTables->pMonPropTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonPropTxt);
		FOG_FreeLinker(sgptDataTables->pMonPropLinker);
	}
	sgptDataTables->nMonPropTxtRecordCount = 0;

	if (sgptDataTables->pMonSoundsTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonSoundsTxt);
		FOG_FreeLinker(sgptDataTables->pMonSoundsLinker);
		sgptDataTables->nMonSoundsTxtRecordCount = 0;
	}

	if (sgptDataTables->pMonSeqTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonSeqTxt);
		FOG_FreeLinker(sgptDataTables->pMonSeqLinker);
		sgptDataTables->nMonSeqTxtRecordCount = 0;
	}
	if (sgptDataTables->pMonSeqTable)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pMonSeqTable);
		sgptDataTables->nMonSeqTableRecordCount = 0;
	}

	FOG_FreeLinker(sgptDataTables->pTreasureClassExLinker);
	for (int i = 0; i < sgptDataTables->nTreasureClassEx; ++i)
	{
		if (sgptDataTables->pTreasureClassEx[i].pInfo)
		{
			D2_FREE_POOL(nullptr, sgptDataTables->pTreasureClassEx[i].pInfo);
		}
	}
	D2_FREE_POOL(nullptr, sgptDataTables->pTreasureClassEx);
	sgptDataTables->pTreasureClassEx = NULL;
	sgptDataTables->nTreasureClassEx = 0;

	if (sgptDataTables->pMonLvlTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pMonLvlTxt);
		sgptDataTables->nMonLvlTxtRecordCount = 0;
	}

	if (sgptDataTables->pUniqueTitleTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pUniqueTitleTxt);
		sgptDataTables->pUniqueTitleTxt = NULL;
		sgptDataTables->nUniqueTitleTxtRecordCount = 0;
	}

	if (sgptDataTables->pUniquePrefixTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pUniquePrefixTxt);
		sgptDataTables->pUniquePrefixTxt = NULL;
		sgptDataTables->nUniquePrefixTxtRecordCount = 0;
	}

	if (sgptDataTables->pUniqueSuffixTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pUniqueSuffixTxt);
		sgptDataTables->pUniqueSuffixTxt = NULL;
		sgptDataTables->nUniqueSuffixTxtRecordCount = 0;
	}

	if (sgptDataTables->pUniqueAppellationTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pUniqueAppellationTxt);
		sgptDataTables->pUniqueAppellationTxt = NULL;
		sgptDataTables->nUniqueAppellationTxtRecordCount = 0;
	}
}
