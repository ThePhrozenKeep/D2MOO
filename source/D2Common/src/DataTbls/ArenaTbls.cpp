#include "D2DataTbls.h"

static const int NUM_ARENA_TYPES = 1;

//D2Common.0x6FD47840
void __fastcall DATATBLS_LoadArenaTxt(HD2ARCHIVE hArchive)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Suicide", TXTFIELD_DWORD, 0, 0, NULL },
		{ "PlayerKill", TXTFIELD_DWORD, 0, 4, NULL },
		{ "PlayerKillPercent", TXTFIELD_DWORD, 0, 8, NULL },
		{ "MonsterKill", TXTFIELD_DWORD, 0, 12, NULL },
		{ "PlayerDeath", TXTFIELD_DWORD, 0, 16, NULL },
		{ "PlayerDeathPercent", TXTFIELD_DWORD, 0, 20, NULL },
		{ "MonsterDeath", TXTFIELD_DWORD, 0, 24, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	gpArenaTxtTable = (D2ArenaTxt*)DATATBLS_CompileTxt(hArchive, "arena", pTbl, NULL, sizeof(D2ArenaTxt));
}

//D2Common.0x6FD47970 (#10596)
D2ArenaTxt* __fastcall DATATBLS_GetArenaTxtRecord(int nArenaType)
{
	D2_ASSERT(nArenaType < NUM_ARENA_TYPES);

	return &gpArenaTxtTable[nArenaType];
}

//D2Common.0x6FD479B0
void __fastcall DATATBLS_UnloadArenaTxt()
{
	DATATBLS_UnloadBin(gpArenaTxtTable);
	gpArenaTxtTable = NULL;
}

//D2Common.0x6FD479D0
void __fastcall DATATBLS_LoadCharTemplateTxt(HD2ARCHIVE hArchive)
{
	int nMaxLevel = 0;
	int nLevel = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "Name", TXTFIELD_ASCII, 29, 0, NULL },
		{ "class", TXTFIELD_CODETOBYTE, 0, 30, &sgptDataTables->pPlayerClassLinker },
		{ "act", TXTFIELD_BYTE, 0, 32, NULL },
		{ "level", TXTFIELD_BYTE, 0, 31, NULL },
		{ "str", TXTFIELD_BYTE, 0, 33, NULL },
		{ "dex", TXTFIELD_BYTE, 0, 34, NULL },
		{ "vit", TXTFIELD_BYTE, 0, 36, NULL },
		{ "int", TXTFIELD_BYTE, 0, 35, NULL },
		{ "Hitpoints", TXTFIELD_BYTE, 0, 38, NULL },
		{ "Mana", TXTFIELD_BYTE, 0, 37, NULL },
		{ "ManaRegenBonus", TXTFIELD_BYTE, 0, 39, NULL },
		{ "Velocity", TXTFIELD_BYTE, 0, 40, NULL },
		{ "AttackRate", TXTFIELD_BYTE, 0, 41, NULL },
		{ "OtherRate", TXTFIELD_BYTE, 0, 42, NULL },
		{ "RightSkill", TXTFIELD_DWORD, 0, 44, NULL },
		{ "Skill1", TXTFIELD_DWORD, 0, 48, NULL },
		{ "SkillLevel1", TXTFIELD_DWORD, 0, 84, NULL },
		{ "Skill2", TXTFIELD_DWORD, 0, 52, NULL },
		{ "SkillLevel2", TXTFIELD_DWORD, 0, 88, NULL },
		{ "Skill3", TXTFIELD_DWORD, 0, 56, NULL },
		{ "SkillLevel3", TXTFIELD_DWORD, 0, 92, NULL },
		{ "Skill4", TXTFIELD_DWORD, 0, 60, NULL },
		{ "SkillLevel4", TXTFIELD_DWORD, 0, 96, NULL },
		{ "Skill5", TXTFIELD_DWORD, 0, 64, NULL },
		{ "SkillLevel5", TXTFIELD_DWORD, 0, 100, NULL },
		{ "Skill6", TXTFIELD_DWORD, 0, 68, NULL },
		{ "SkillLevel6", TXTFIELD_DWORD, 0, 104, NULL },
		{ "Skill7", TXTFIELD_DWORD, 0, 72, NULL },
		{ "SkillLevel7", TXTFIELD_DWORD, 0, 108, NULL },
		{ "Skill8", TXTFIELD_DWORD, 0, 76, NULL },
		{ "SkillLevel8", TXTFIELD_DWORD, 0, 112, NULL },
		{ "Skill9", TXTFIELD_DWORD, 0, 80, NULL },
		{ "SkillLevel9", TXTFIELD_DWORD, 0, 116, NULL },
		{ "item1", TXTFIELD_RAW, 0, 120, NULL },
		{ "item1loc", TXTFIELD_BYTE, 0, 124, NULL },
		{ "item1count", TXTFIELD_BYTE, 0, 125, NULL },
		{ "item2", TXTFIELD_RAW, 0, 128, NULL },
		{ "item2loc", TXTFIELD_BYTE, 0, 132, NULL },
		{ "item2count", TXTFIELD_BYTE, 0, 133, NULL },
		{ "item3", TXTFIELD_RAW, 0, 136, NULL },
		{ "item3loc", TXTFIELD_BYTE, 0, 140, NULL },
		{ "item3count", TXTFIELD_BYTE, 0, 141, NULL },
		{ "item4", TXTFIELD_RAW, 0, 144, NULL },
		{ "item4loc", TXTFIELD_BYTE, 0, 148, NULL },
		{ "item4count", TXTFIELD_BYTE, 0, 149, NULL },
		{ "item5", TXTFIELD_RAW, 0, 152, NULL },
		{ "item5loc", TXTFIELD_BYTE, 0, 156, NULL },
		{ "item5count", TXTFIELD_BYTE, 0, 157, NULL },
		{ "item6", TXTFIELD_RAW, 0, 160, NULL },
		{ "item6loc", TXTFIELD_BYTE, 0, 164, NULL },
		{ "item6count", TXTFIELD_BYTE, 0, 165, NULL },
		{ "item7", TXTFIELD_RAW, 0, 168, NULL },
		{ "item7loc", TXTFIELD_BYTE, 0, 172, NULL },
		{ "item7count", TXTFIELD_BYTE, 0, 173, NULL },
		{ "item8", TXTFIELD_RAW, 0, 176, NULL },
		{ "item8loc", TXTFIELD_BYTE, 0, 180, NULL },
		{ "item8count", TXTFIELD_BYTE, 0, 181, NULL },
		{ "item9", TXTFIELD_RAW, 0, 184, NULL },
		{ "item9loc", TXTFIELD_BYTE, 0, 188, NULL },
		{ "item9count", TXTFIELD_BYTE, 0, 189, NULL },
		{ "item10", TXTFIELD_RAW, 0, 192, NULL },
		{ "item10loc", TXTFIELD_BYTE, 0, 196, NULL },
		{ "item10count", TXTFIELD_BYTE, 0, 197, NULL },
		{ "item11", TXTFIELD_RAW, 0, 200, NULL },
		{ "item11loc", TXTFIELD_BYTE, 0, 204, NULL },
		{ "item11count", TXTFIELD_BYTE, 0, 205, NULL },
		{ "item12", TXTFIELD_RAW, 0, 208, NULL },
		{ "item12loc", TXTFIELD_BYTE, 0, 212, NULL },
		{ "item12count", TXTFIELD_BYTE, 0, 213, NULL },
		{ "item13", TXTFIELD_RAW, 0, 216, NULL },
		{ "item13loc", TXTFIELD_BYTE, 0, 220, NULL },
		{ "item13count", TXTFIELD_BYTE, 0, 221, NULL },
		{ "item14", TXTFIELD_RAW, 0, 224, NULL },
		{ "item14loc", TXTFIELD_BYTE, 0, 228, NULL },
		{ "item14count", TXTFIELD_BYTE, 0, 229, NULL },
		{ "item15", TXTFIELD_RAW, 0, 232, NULL },
		{ "item15loc", TXTFIELD_BYTE, 0, 236, NULL },
		{ "item15count", TXTFIELD_BYTE, 0, 237, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	gpCharTemplateTxtTable = (D2CharTemplateTxt*)DATATBLS_CompileTxt(hArchive, "chartemplate", pTbl, &gnCharTemplateTxtTableRecordCount, sizeof(D2CharTemplateTxt));

	memset(gnCharTemplateStartIds, 0x00, sizeof(gnCharTemplateStartIds));

	for (int i = 0; i < gnCharTemplateTxtTableRecordCount; ++i)
	{
		nLevel = gpCharTemplateTxtTable[i].nLevel;

		D2_ASSERT(nLevel >= nMaxLevel);

		if (nLevel > nMaxLevel)
		{
			gnCharTemplateStartIds[nLevel] = i;
			nMaxLevel = nLevel;
		}
	}
}

//D2Common.0x6FD48770
void __fastcall DATATBLS_UnloadCharTemplateTxt()
{
	DATATBLS_UnloadBin(gpCharTemplateTxtTable);
	gpCharTemplateTxtTable = NULL;
}

//D2Common.0x6FD48790 (#10664)
int __fastcall DATATBLS_GetCharTemplateTxtRecordCount()
{
	return gnCharTemplateTxtTableRecordCount;
}

//D2Common.0x6FD487A0 (#10665)
D2CharTemplateTxt* __fastcall DATATBLS_GetCharTemplateTxtRecord(int nTemplate, int nLevel)
{
	D2_ASSERT(nTemplate > 0);
	const int nIndex = gnCharTemplateStartIds[nLevel] + nTemplate - 1;
	D2_ASSERT(nIndex < gnCharTemplateTxtTableRecordCount);

	return &gpCharTemplateTxtTable[nIndex];
}

//D2Common.0x6FD48810 (#10666)
int __fastcall DATATBLS_GetClassFromCharTemplateTxtRecord(int nTemplate, int nLevel)
{
	return DATATBLS_GetCharTemplateTxtRecord(nTemplate, nLevel)->nClass;
}
