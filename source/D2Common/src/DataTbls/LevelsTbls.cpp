#include "D2DataTbls.h"

#include "Drlg/D2DrlgMaze.h"
#include "Drlg/D2DrlgPreset.h"
#include "Drlg/D2DrlgTileSub.h"
#include "D2Seed.h"
#include <D2Lang.h>
#include <D2CMP.h>


static const char* gszAutomapLevelNames[] =
{
	"None",
	"1 Town",
	"1 Wilderness",
	"1 Cave",
	"1 Crypt",
	"1 Monestary",
	"1 Courtyard",
	"1 Barracks",
	"1 Jail",
	"1 Cathedral",
	"1 Catacombs",
	"1 Tristram",
	"2 Town",
	"2 Sewer",
	"2 Harem",
	"2 Basement",
	"2 Desert",
	"2 Tomb",
	"2 Lair",
	"2 Arcane",
	"3 Town",
	"3 Jungle",
	"3 Kurast",
	"3 Spider",
	"3 Dungeon",
	"3 Sewer",
	"4 Town",
	"4 Mesa",
	"4 Lava",
	"5 Town",
	"5 Siege",
	"5 Barricade",
	"5 Temple",
	"5 Ice",
	"5 Baal",
	"5 Lava"
};

static const char* gszAutomapTileNames[] =
{
	"fl",
	"wl",
	"wr",
	"wtlr",
	"wtll",
	"wtr",
	"wbl",
	"wbr",
	"wld",
	"wrd",
	"wle",
	"wre",
	"co",
	"sh",
	"tr",
	"rf",
	"ld",
	"rd",
	"fd",
	"fi"
};

D2SeedStrc* gpAutomapSeed = &sgptDataTables->pAutomapSeed;

//D2Common.0x6FDEA704
LPCRITICAL_SECTION gpLvlSubTypeFilesCriticalSection;

//D2Common.0x6FD5EAE0
void __fastcall DATATBLS_LoadLevelsTxt(void* pMemPool)
{
	int nMonsterCounter = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "Id", TXTFIELD_BYTE, 0, 0, NULL },
		{ "Pal", TXTFIELD_BYTE, 0, 2, NULL },
		{ "Act", TXTFIELD_BYTE, 0, 3, NULL },
		{ "Teleport", TXTFIELD_BYTE, 0, 4, NULL },
		{ "Rain", TXTFIELD_BYTE, 0, 5, NULL },
		{ "Mud", TXTFIELD_BYTE, 0, 6, NULL },
		{ "NoPer", TXTFIELD_BYTE, 0, 7, NULL },
		{ "IsInside", TXTFIELD_BYTE, 0, 8, NULL },
		{ "DrawEdges", TXTFIELD_BYTE, 0, 9, NULL },
		{ "WarpDist", TXTFIELD_DWORD, 0, 12, NULL },
		{ "MonLvl1", TXTFIELD_WORD, 0, 16, NULL },
		{ "MonLvl2", TXTFIELD_WORD, 0, 18, NULL },
		{ "MonLvl3", TXTFIELD_WORD, 0, 20, NULL },
		{ "MonLvl1Ex", TXTFIELD_WORD, 0, 22, NULL },
		{ "MonLvl2Ex", TXTFIELD_WORD, 0, 24, NULL },
		{ "MonLvl3Ex", TXTFIELD_WORD, 0, 26, NULL },
		{ "MonDen", TXTFIELD_DWORD, 0, 28, NULL },
		{ "MonDen(N)", TXTFIELD_DWORD, 0, 32, NULL },
		{ "MonDen(H)", TXTFIELD_DWORD, 0, 36, NULL },
		{ "MonUMin", TXTFIELD_BYTE, 0, 40, NULL },
		{ "MonUMax", TXTFIELD_BYTE, 0, 43, NULL },
		{ "MonUMin(N)", TXTFIELD_BYTE, 0, 41, NULL },
		{ "MonUMax(N)", TXTFIELD_BYTE, 0, 44, NULL },
		{ "MonUMin(H)", TXTFIELD_BYTE, 0, 42, NULL },
		{ "MonUMax(H)", TXTFIELD_BYTE, 0, 45, NULL },
		{ "MonWndr", TXTFIELD_BYTE, 0, 46, NULL },
		{ "MonSpcWalk", TXTFIELD_BYTE, 0, 47, NULL },
		{ "ObjGrp0", TXTFIELD_BYTE, 0, 229, NULL },
		{ "ObjGrp1", TXTFIELD_BYTE, 0, 230, NULL },
		{ "ObjGrp2", TXTFIELD_BYTE, 0, 231, NULL },
		{ "ObjGrp3", TXTFIELD_BYTE, 0, 232, NULL },
		{ "ObjGrp4", TXTFIELD_BYTE, 0, 233, NULL },
		{ "ObjGrp5", TXTFIELD_BYTE, 0, 234, NULL },
		{ "ObjGrp6", TXTFIELD_BYTE, 0, 235, NULL },
		{ "ObjGrp7", TXTFIELD_BYTE, 0, 236, NULL },
		{ "ObjPrb0", TXTFIELD_BYTE, 0, 237, NULL },
		{ "ObjPrb1", TXTFIELD_BYTE, 0, 238, NULL },
		{ "ObjPrb2", TXTFIELD_BYTE, 0, 239, NULL },
		{ "ObjPrb3", TXTFIELD_BYTE, 0, 240, NULL },
		{ "ObjPrb4", TXTFIELD_BYTE, 0, 241, NULL },
		{ "ObjPrb5", TXTFIELD_BYTE, 0, 242, NULL },
		{ "ObjPrb6", TXTFIELD_BYTE, 0, 243, NULL },
		{ "ObjPrb7", TXTFIELD_BYTE, 0, 244, NULL },
		{ "Quest", TXTFIELD_BYTE, 0, 48, NULL },
		{ "NumMon", TXTFIELD_BYTE, 0, 50, NULL },
		{ "mon1", TXTFIELD_NAMETOWORD, 0, 54, &sgptDataTables->pMonStatsLinker },
		{ "mon2", TXTFIELD_NAMETOWORD, 0, 56, &sgptDataTables->pMonStatsLinker },
		{ "mon3", TXTFIELD_NAMETOWORD, 0, 58, &sgptDataTables->pMonStatsLinker },
		{ "mon4", TXTFIELD_NAMETOWORD, 0, 60, &sgptDataTables->pMonStatsLinker },
		{ "mon5", TXTFIELD_NAMETOWORD, 0, 62, &sgptDataTables->pMonStatsLinker },
		{ "mon6", TXTFIELD_NAMETOWORD, 0, 64, &sgptDataTables->pMonStatsLinker },
		{ "mon7", TXTFIELD_NAMETOWORD, 0, 66, &sgptDataTables->pMonStatsLinker },
		{ "mon8", TXTFIELD_NAMETOWORD, 0, 68, &sgptDataTables->pMonStatsLinker },
		{ "mon9", TXTFIELD_NAMETOWORD, 0, 70, &sgptDataTables->pMonStatsLinker },
		{ "mon10", TXTFIELD_NAMETOWORD, 0, 72, &sgptDataTables->pMonStatsLinker },
		{ "mon11", TXTFIELD_NAMETOWORD, 0, 74, &sgptDataTables->pMonStatsLinker },
		{ "mon12", TXTFIELD_NAMETOWORD, 0, 76, &sgptDataTables->pMonStatsLinker },
		{ "mon13", TXTFIELD_NAMETOWORD, 0, 78, &sgptDataTables->pMonStatsLinker },
		{ "mon14", TXTFIELD_NAMETOWORD, 0, 80, &sgptDataTables->pMonStatsLinker },
		{ "mon15", TXTFIELD_NAMETOWORD, 0, 82, &sgptDataTables->pMonStatsLinker },
		{ "mon16", TXTFIELD_NAMETOWORD, 0, 84, &sgptDataTables->pMonStatsLinker },
		{ "mon17", TXTFIELD_NAMETOWORD, 0, 86, &sgptDataTables->pMonStatsLinker },
		{ "mon18", TXTFIELD_NAMETOWORD, 0, 88, &sgptDataTables->pMonStatsLinker },
		{ "mon19", TXTFIELD_NAMETOWORD, 0, 90, &sgptDataTables->pMonStatsLinker },
		{ "mon20", TXTFIELD_NAMETOWORD, 0, 92, &sgptDataTables->pMonStatsLinker },
		{ "mon21", TXTFIELD_NAMETOWORD, 0, 94, &sgptDataTables->pMonStatsLinker },
		{ "mon22", TXTFIELD_NAMETOWORD, 0, 96, &sgptDataTables->pMonStatsLinker },
		{ "mon23", TXTFIELD_NAMETOWORD, 0, 98, &sgptDataTables->pMonStatsLinker },
		{ "mon24", TXTFIELD_NAMETOWORD, 0, 100, &sgptDataTables->pMonStatsLinker },
		{ "mon25", TXTFIELD_NAMETOWORD, 0, 102, &sgptDataTables->pMonStatsLinker },
		{ "rangedspawn", TXTFIELD_BYTE, 0, 49, NULL },
		{ "nmon1", TXTFIELD_NAMETOWORD, 0, 104, &sgptDataTables->pMonStatsLinker },
		{ "nmon2", TXTFIELD_NAMETOWORD, 0, 106, &sgptDataTables->pMonStatsLinker },
		{ "nmon3", TXTFIELD_NAMETOWORD, 0, 108, &sgptDataTables->pMonStatsLinker },
		{ "nmon4", TXTFIELD_NAMETOWORD, 0, 110, &sgptDataTables->pMonStatsLinker },
		{ "nmon5", TXTFIELD_NAMETOWORD, 0, 112, &sgptDataTables->pMonStatsLinker },
		{ "nmon6", TXTFIELD_NAMETOWORD, 0, 114, &sgptDataTables->pMonStatsLinker },
		{ "nmon7", TXTFIELD_NAMETOWORD, 0, 116, &sgptDataTables->pMonStatsLinker },
		{ "nmon8", TXTFIELD_NAMETOWORD, 0, 118, &sgptDataTables->pMonStatsLinker },
		{ "nmon9", TXTFIELD_NAMETOWORD, 0, 120, &sgptDataTables->pMonStatsLinker },
		{ "nmon10", TXTFIELD_NAMETOWORD, 0, 122, &sgptDataTables->pMonStatsLinker },
		{ "nmon11", TXTFIELD_NAMETOWORD, 0, 124, &sgptDataTables->pMonStatsLinker },
		{ "nmon12", TXTFIELD_NAMETOWORD, 0, 126, &sgptDataTables->pMonStatsLinker },
		{ "nmon13", TXTFIELD_NAMETOWORD, 0, 128, &sgptDataTables->pMonStatsLinker },
		{ "nmon14", TXTFIELD_NAMETOWORD, 0, 130, &sgptDataTables->pMonStatsLinker },
		{ "nmon15", TXTFIELD_NAMETOWORD, 0, 132, &sgptDataTables->pMonStatsLinker },
		{ "nmon16", TXTFIELD_NAMETOWORD, 0, 134, &sgptDataTables->pMonStatsLinker },
		{ "nmon17", TXTFIELD_NAMETOWORD, 0, 136, &sgptDataTables->pMonStatsLinker },
		{ "nmon18", TXTFIELD_NAMETOWORD, 0, 138, &sgptDataTables->pMonStatsLinker },
		{ "nmon19", TXTFIELD_NAMETOWORD, 0, 140, &sgptDataTables->pMonStatsLinker },
		{ "nmon20", TXTFIELD_NAMETOWORD, 0, 142, &sgptDataTables->pMonStatsLinker },
		{ "nmon21", TXTFIELD_NAMETOWORD, 0, 144, &sgptDataTables->pMonStatsLinker },
		{ "nmon22", TXTFIELD_NAMETOWORD, 0, 146, &sgptDataTables->pMonStatsLinker },
		{ "nmon23", TXTFIELD_NAMETOWORD, 0, 148, &sgptDataTables->pMonStatsLinker },
		{ "nmon24", TXTFIELD_NAMETOWORD, 0, 150, &sgptDataTables->pMonStatsLinker },
		{ "nmon25", TXTFIELD_NAMETOWORD, 0, 152, &sgptDataTables->pMonStatsLinker },
		{ "umon1", TXTFIELD_NAMETOWORD, 0, 154, &sgptDataTables->pMonStatsLinker },
		{ "umon2", TXTFIELD_NAMETOWORD, 0, 156, &sgptDataTables->pMonStatsLinker },
		{ "umon3", TXTFIELD_NAMETOWORD, 0, 158, &sgptDataTables->pMonStatsLinker },
		{ "umon4", TXTFIELD_NAMETOWORD, 0, 160, &sgptDataTables->pMonStatsLinker },
		{ "umon5", TXTFIELD_NAMETOWORD, 0, 162, &sgptDataTables->pMonStatsLinker },
		{ "umon6", TXTFIELD_NAMETOWORD, 0, 164, &sgptDataTables->pMonStatsLinker },
		{ "umon7", TXTFIELD_NAMETOWORD, 0, 166, &sgptDataTables->pMonStatsLinker },
		{ "umon8", TXTFIELD_NAMETOWORD, 0, 168, &sgptDataTables->pMonStatsLinker },
		{ "umon9", TXTFIELD_NAMETOWORD, 0, 170, &sgptDataTables->pMonStatsLinker },
		{ "umon10", TXTFIELD_NAMETOWORD, 0, 172, &sgptDataTables->pMonStatsLinker },
		{ "umon11", TXTFIELD_NAMETOWORD, 0, 174, &sgptDataTables->pMonStatsLinker },
		{ "umon12", TXTFIELD_NAMETOWORD, 0, 176, &sgptDataTables->pMonStatsLinker },
		{ "umon13", TXTFIELD_NAMETOWORD, 0, 178, &sgptDataTables->pMonStatsLinker },
		{ "umon14", TXTFIELD_NAMETOWORD, 0, 180, &sgptDataTables->pMonStatsLinker },
		{ "umon15", TXTFIELD_NAMETOWORD, 0, 182, &sgptDataTables->pMonStatsLinker },
		{ "umon16", TXTFIELD_NAMETOWORD, 0, 184, &sgptDataTables->pMonStatsLinker },
		{ "umon17", TXTFIELD_NAMETOWORD, 0, 186, &sgptDataTables->pMonStatsLinker },
		{ "umon18", TXTFIELD_NAMETOWORD, 0, 188, &sgptDataTables->pMonStatsLinker },
		{ "umon19", TXTFIELD_NAMETOWORD, 0, 190, &sgptDataTables->pMonStatsLinker },
		{ "umon20", TXTFIELD_NAMETOWORD, 0, 192, &sgptDataTables->pMonStatsLinker },
		{ "umon21", TXTFIELD_NAMETOWORD, 0, 194, &sgptDataTables->pMonStatsLinker },
		{ "umon22", TXTFIELD_NAMETOWORD, 0, 196, &sgptDataTables->pMonStatsLinker },
		{ "umon23", TXTFIELD_NAMETOWORD, 0, 198, &sgptDataTables->pMonStatsLinker },
		{ "umon24", TXTFIELD_NAMETOWORD, 0, 200, &sgptDataTables->pMonStatsLinker },
		{ "umon25", TXTFIELD_NAMETOWORD, 0, 202, &sgptDataTables->pMonStatsLinker },
		{ "cmon1", TXTFIELD_NAMETOWORD, 0, 204, &sgptDataTables->pMonStatsLinker },
		{ "cmon2", TXTFIELD_NAMETOWORD, 0, 206, &sgptDataTables->pMonStatsLinker },
		{ "cmon3", TXTFIELD_NAMETOWORD, 0, 208, &sgptDataTables->pMonStatsLinker },
		{ "cmon4", TXTFIELD_NAMETOWORD, 0, 210, &sgptDataTables->pMonStatsLinker },
		{ "cpct1", TXTFIELD_WORD, 0, 212, NULL },
		{ "cpct2", TXTFIELD_WORD, 0, 214, NULL },
		{ "cpct3", TXTFIELD_WORD, 0, 216, NULL },
		{ "cpct4", TXTFIELD_WORD, 0, 218, NULL },
		{ "camt1", TXTFIELD_WORD, 0, 220, NULL },
		{ "camt2", TXTFIELD_WORD, 0, 220, NULL },
		{ "camt3", TXTFIELD_WORD, 0, 220, NULL },
		{ "camt4", TXTFIELD_WORD, 0, 220, NULL },
		{ "Themes", TXTFIELD_DWORD, 0, 528, NULL },
		{ "FloorFilter", TXTFIELD_DWORD, 0, 532, NULL },
		{ "BlankScreen", TXTFIELD_DWORD, 0, 536, NULL },
		{ "SoundEnv", TXTFIELD_BYTE, 0, 540, NULL },
		{ "Waypoint", TXTFIELD_BYTE, 0, 228, NULL },
		{ "LevelName", TXTFIELD_ASCII, 39, 245, NULL },
		{ "LevelWarp", TXTFIELD_ASCII, 39, 285, NULL },
		{ "EntryFile", TXTFIELD_ASCII, 39, 325, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pLevelsTxt = (D2LevelsTxt*)DATATBLS_CompileTxt(pMemPool, "levels", pTbl, &sgptDataTables->nLevelsTxtRecordCount, sizeof(D2LevelsTxt));

#define MAX_LEVEL_ID 1024
	D2_ASSERT(sgptDataTables->nLevelsTxtRecordCount < MAX_LEVEL_ID);

	for (int i = 0; i < sgptDataTables->nLevelsTxtRecordCount; ++i)
	{
		memset(sgptDataTables->pLevelsTxt[i].wszLevelName, 0x00, sizeof(sgptDataTables->pLevelsTxt[i].wszLevelName));
		memset(sgptDataTables->pLevelsTxt[i].wszLevelWarp, 0x00, sizeof(sgptDataTables->pLevelsTxt[i].wszLevelWarp));

		wcsncpy_s(sgptDataTables->pLevelsTxt[i].wszLevelName, D2LANG_GetStringByReferenceString(sgptDataTables->pLevelsTxt[i].szLevelName), sizeof(sgptDataTables->pLevelsTxt[i].szLevelName));
		sgptDataTables->pLevelsTxt[i].wszLevelName[39] = 0;

		wcsncpy_s(sgptDataTables->pLevelsTxt[i].wszLevelWarp, D2LANG_GetStringByReferenceString(sgptDataTables->pLevelsTxt[i].szLevelWarp), sizeof(sgptDataTables->pLevelsTxt[i].szLevelWarp));
		sgptDataTables->pLevelsTxt[i].wszLevelWarp[39] = 0;

		nMonsterCounter = 0;
		do
		{
			if (sgptDataTables->pLevelsTxt[i].wMon[nMonsterCounter] < 0)
			{
				break;
			}
			++nMonsterCounter;
		}
		while (nMonsterCounter < 25);
		sgptDataTables->pLevelsTxt[i].nNumNormMon = nMonsterCounter;

		nMonsterCounter = 0;
		do
		{
			if (sgptDataTables->pLevelsTxt[i].wNMon[nMonsterCounter] < 0)
			{
				break;
			}
			++nMonsterCounter;
		}
		while (nMonsterCounter < 25);
		sgptDataTables->pLevelsTxt[i].nNumNMon = nMonsterCounter;

		nMonsterCounter = 0;
		do
		{
			if (sgptDataTables->pLevelsTxt[i].wUMon[nMonsterCounter] < 0)
			{
				break;
			}
			++nMonsterCounter;
		}
		while (nMonsterCounter < 25);
		sgptDataTables->pLevelsTxt[i].nNumUMon = nMonsterCounter;
	}
}

//D2Common.0x6FD603C0 (#10631)
D2LevelsTxt* __stdcall DATATBLS_GetLevelsTxtRecord(int nLevelId)
{
	if (nLevelId >= 0 && nLevelId < sgptDataTables->nLevelsTxtRecordCount)
	{
		return &sgptDataTables->pLevelsTxt[nLevelId];
	}

	return NULL;
}

//D2Common.0x6FD603F0 (#10632)
uint8_t __stdcall DATATBLS_GetRainFromLevelsTxt(int nLevelId)
{
	if (nLevelId >= 0 && nLevelId < sgptDataTables->nLevelsTxtRecordCount)
	{
		return sgptDataTables->pLevelsTxt[nLevelId].nRain;
	}

	return 0;
}

//D2Common.0x6FD60430 (#10634)
uint8_t __stdcall DATATBLS_GetNoPerFromLevelsTxt(int nLevelId)
{
	if (nLevelId >= 0 && nLevelId < sgptDataTables->nLevelsTxtRecordCount)
	{
		return sgptDataTables->pLevelsTxt[nLevelId].nNoPer;
	}

	return 0;
}

//D2Common.0x6FD60470 (#10633)
uint8_t __stdcall DATATBLS_GetMudFromLevelsTxt(int nLevelId)
{
	if (nLevelId >= 0 && nLevelId < sgptDataTables->nLevelsTxtRecordCount)
	{
		return sgptDataTables->pLevelsTxt[nLevelId].nMud;
	}

	return 0;
}

//D2Common.0x6FD604B0
void __fastcall DATATBLS_UnloadLevelsTxt()
{
	if (sgptDataTables->pLevelsTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pLevelsTxt);
	}
	sgptDataTables->nLevelsTxtRecordCount = 0;
}

//D2Common.0x6FD604F0 (#11247)
int __stdcall DATATBLS_GetMonsterLevelInArea(int nLevelId, uint8_t nDifficulty, BOOL bExpansion)
{
	D2LevelsTxt* pLevelsTxtRecord = NULL;

	if (nDifficulty < 3 && nLevelId > 0 && nLevelId < sgptDataTables->nLevelsTxtRecordCount)
	{
		pLevelsTxtRecord = &sgptDataTables->pLevelsTxt[nLevelId];
		if (pLevelsTxtRecord)
		{
			if (bExpansion)
			{
				return pLevelsTxtRecord->wMonLvlEx[nDifficulty];
			}
			else
			{
				return pLevelsTxtRecord->wMonLvl[nDifficulty];
			}
		}
	}

	return 1;
}

//D2Common.0x6FD60560
int* __fastcall DATATBLS_GetPortalLevels(int* pnPortalLevels)
{
	*pnPortalLevels = sgptDataTables->nPortalLevels;
	return sgptDataTables->pPortalLevels;
}

//D2Common.0x6FD60570
void __fastcall DATATBLS_LoadLevelDefsBin(void* pMemPool)
{
	int nPortalIndex = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "QuestFlag", TXTFIELD_DWORD, 0, 0, NULL },
		{ "QuestFlagEx", TXTFIELD_DWORD, 0, 4, NULL },
		{ "Layer", TXTFIELD_DWORD, 0, 8, NULL },
		{ "SizeX", TXTFIELD_DWORD, 0, 12, NULL },
		{ "SizeY", TXTFIELD_DWORD, 0, 24, NULL },
		{ "SizeX(N)", TXTFIELD_DWORD, 0, 16, NULL },
		{ "SizeY(N)", TXTFIELD_DWORD, 0, 28, NULL },
		{ "SizeX(H)", TXTFIELD_DWORD, 0, 20, NULL },
		{ "SizeY(H)", TXTFIELD_DWORD, 0, 32, NULL },
		{ "OffsetX", TXTFIELD_DWORD, 0, 36, NULL },
		{ "OffsetY", TXTFIELD_DWORD, 0, 40, NULL },
		{ "Depend", TXTFIELD_DWORD, 0, 44, NULL },
		{ "DrlgType", TXTFIELD_DWORD, 0, 48, NULL },
		{ "LevelType", TXTFIELD_DWORD, 0, 52, NULL },
		{ "SubType", TXTFIELD_DWORD, 0, 56, NULL },
		{ "SubTheme", TXTFIELD_DWORD, 0, 60, NULL },
		{ "SubWaypoint", TXTFIELD_DWORD, 0, 64, NULL },
		{ "SubShrine", TXTFIELD_DWORD, 0, 68, NULL },
		{ "Vis0", TXTFIELD_DWORD, 0, 72, NULL },
		{ "Vis1", TXTFIELD_DWORD, 0, 76, NULL },
		{ "Vis2", TXTFIELD_DWORD, 0, 80, NULL },
		{ "Vis3", TXTFIELD_DWORD, 0, 84, NULL },
		{ "Vis4", TXTFIELD_DWORD, 0, 88, NULL },
		{ "Vis5", TXTFIELD_DWORD, 0, 92, NULL },
		{ "Vis6", TXTFIELD_DWORD, 0, 96, NULL },
		{ "Vis7", TXTFIELD_DWORD, 0, 100, NULL },
		{ "Warp0", TXTFIELD_DWORD, 0, 104, NULL },
		{ "Warp1", TXTFIELD_DWORD, 0, 108, NULL },
		{ "Warp2", TXTFIELD_DWORD, 0, 112, NULL },
		{ "Warp3", TXTFIELD_DWORD, 0, 116, NULL },
		{ "Warp4", TXTFIELD_DWORD, 0, 120, NULL },
		{ "Warp5", TXTFIELD_DWORD, 0, 124, NULL },
		{ "Warp6", TXTFIELD_DWORD, 0, 128, NULL },
		{ "Warp7", TXTFIELD_DWORD, 0, 132, NULL },
		{ "Intensity", TXTFIELD_BYTE, 0, 136, NULL },
		{ "Red", TXTFIELD_BYTE, 0, 137, NULL },
		{ "Green", TXTFIELD_BYTE, 0, 138, NULL },
		{ "Blue", TXTFIELD_BYTE, 0, 139, NULL },
		{ "Portal", TXTFIELD_DWORD, 0, 140, NULL },
		{ "Position", TXTFIELD_DWORD, 0, 144, NULL },
		{ "SaveMonsters", TXTFIELD_DWORD, 0, 148, NULL },
		{ "LOSDraw", TXTFIELD_DWORD, 0, 152, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pLevelDefBin = (D2LevelDefBin*)DATATBLS_CompileTxt(pMemPool, "leveldefs", pTbl, 0, sizeof(D2LevelDefBin));

	sgptDataTables->nPortalLevels = 0;
	sgptDataTables->pPortalLevels = NULL;

	for (int i = 0; i < sgptDataTables->nLevelsTxtRecordCount; ++i)
	{
		if (sgptDataTables->pLevelDefBin[i].dwPortal)
		{
			++sgptDataTables->nPortalLevels;
		}
	}

	if (sgptDataTables->nPortalLevels)
	{
		sgptDataTables->pPortalLevels = (int*)D2_CALLOC_POOL(nullptr, sizeof(int) * sgptDataTables->nPortalLevels);
	}

	nPortalIndex = 0;
	for (int i = 0; i < sgptDataTables->nLevelsTxtRecordCount; ++i)
	{
		if (sgptDataTables->pLevelDefBin[i].dwPortal)
		{
			D2_ASSERT(nPortalIndex < sgptDataTables->nPortalLevels);
			sgptDataTables->pPortalLevels[nPortalIndex] = i;
			++nPortalIndex;
		}
	}
}

//D2Common.0x6FD60D60
void __fastcall DATATBLS_UnloadLevelDefsBin()
{
	if (sgptDataTables->pLevelDefBin)
	{
		DATATBLS_UnloadBin(sgptDataTables->pLevelDefBin);
	}

	if (sgptDataTables->pPortalLevels)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pPortalLevels);
	}
}

//D2Common.0x6FD60D90 (#10010)
D2LevelDefBin* __fastcall DATATBLS_GetLevelDefRecord(int nLevelId)
{
	if (nLevelId >= 0 && nLevelId < sgptDataTables->nLevelsTxtRecordCount)
	{
		return &sgptDataTables->pLevelDefBin[nLevelId];
	}

	return NULL;
}

//D2Common.0x6FD60DC0
void __fastcall DATATBLS_LoadLevelTypesTxt(void* pMemPool)
{	
	int nLen = 0;
	char szFile[60] = {};

	D2BinFieldStrc pTbl[] =
	{
		{ "File 1", TXTFIELD_ASCII, 59, 0, NULL },
		{ "File 2", TXTFIELD_ASCII, 59, 60, NULL },
		{ "File 3", TXTFIELD_ASCII, 59, 120, NULL },
		{ "File 4", TXTFIELD_ASCII, 59, 180, NULL },
		{ "File 5", TXTFIELD_ASCII, 59, 240, NULL },
		{ "File 6", TXTFIELD_ASCII, 59, 300, NULL },
		{ "File 7", TXTFIELD_ASCII, 59, 360, NULL },
		{ "File 8", TXTFIELD_ASCII, 59, 420, NULL },
		{ "File 9", TXTFIELD_ASCII, 59, 480, NULL },
		{ "File 10", TXTFIELD_ASCII, 59, 540, NULL },
		{ "File 11", TXTFIELD_ASCII, 59, 600, NULL },
		{ "File 12", TXTFIELD_ASCII, 59, 660, NULL },
		{ "File 13", TXTFIELD_ASCII, 59, 720, NULL },
		{ "File 14", TXTFIELD_ASCII, 59, 780, NULL },
		{ "File 15", TXTFIELD_ASCII, 59, 840, NULL },
		{ "File 16", TXTFIELD_ASCII, 59, 900, NULL },
		{ "File 17", TXTFIELD_ASCII, 59, 960, NULL },
		{ "File 18", TXTFIELD_ASCII, 59, 1020, NULL },
		{ "File 19", TXTFIELD_ASCII, 59, 1080, NULL },
		{ "File 20", TXTFIELD_ASCII, 59, 1140, NULL },
		{ "File 21", TXTFIELD_ASCII, 59, 1200, NULL },
		{ "File 22", TXTFIELD_ASCII, 59, 1260, NULL },
		{ "File 23", TXTFIELD_ASCII, 59, 1320, NULL },
		{ "File 24", TXTFIELD_ASCII, 59, 1380, NULL },
		{ "File 25", TXTFIELD_ASCII, 59, 1440, NULL },
		{ "File 26", TXTFIELD_ASCII, 59, 1500, NULL },
		{ "File 27", TXTFIELD_ASCII, 59, 1560, NULL },
		{ "File 28", TXTFIELD_ASCII, 59, 1620, NULL },
		{ "File 29", TXTFIELD_ASCII, 59, 1680, NULL },
		{ "File 30", TXTFIELD_ASCII, 59, 1740, NULL },
		{ "File 31", TXTFIELD_ASCII, 59, 1800, NULL },
		{ "File 32", TXTFIELD_ASCII, 59, 1860, NULL },
		{ "Act", TXTFIELD_BYTE, 0, 1920, NULL },
		{ "Expansion", TXTFIELD_DWORD, 0, 1924, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pLvlTypesTxt = (D2LvlTypesTxt*)DATATBLS_CompileTxt(pMemPool, "lvltypes", pTbl, &sgptDataTables->nLvlTypesTxtRecordCount, sizeof(D2LvlTypesTxt));

	for (int i = 0; i < sgptDataTables->nLvlTypesTxtRecordCount; ++i)
	{
		for (int j = 0; j < 32; ++j)
		{
			nLen = strlen(sgptDataTables->pLvlTypesTxt[i].szFile[j]);

			for (int k = 0; k < nLen; ++k)
			{
				if (sgptDataTables->pLvlTypesTxt[i].szFile[j][k] == '/')
				{
					sgptDataTables->pLvlTypesTxt[i].szFile[j][k] = '\\';
				}
			}

			if (nLen > 1)
			{
				wsprintfA(szFile, "%s\\%s", "DATA\\GLOBAL\\TILES", sgptDataTables->pLvlTypesTxt[i].szFile[j]);
				strcpy_s(sgptDataTables->pLvlTypesTxt[i].szFile[j], sizeof(sgptDataTables->pLvlTypesTxt[i].szFile[j]), szFile);
			}
		}
	}
}

//D2Common.0x6FD61450
void __fastcall DATATBLS_UnloadLevelTypesTxt()
{
	DATATBLS_UnloadBin(sgptDataTables->pLvlTypesTxt);
}

//D2Common.0x6FD61460 (#10023)
D2LvlTypesTxt* __stdcall DATATBLS_GetLevelTypesTxtRecord(int nLevelType)
{
	D2_ASSERT(sgptDataTables->pLvlTypesTxt);
	return &sgptDataTables->pLvlTypesTxt[nLevelType];
}

//D2Common.0x6FD614A0 (#11226)
bool __stdcall DATATBLS_CheckActInLevelTypesTxt(int nLevelType, uint8_t nAct)
{
	D2LvlTypesTxt* pLevelTypesTxtRecord = NULL;

	D2_ASSERT(sgptDataTables->pLvlTypesTxt);

	pLevelTypesTxtRecord = &sgptDataTables->pLvlTypesTxt[nLevelType];

	return pLevelTypesTxtRecord != NULL && pLevelTypesTxtRecord->nAct == nAct;
}

//D2Common.0x6FD61500 (#11227)
void __stdcall DATATBLS_GetFileNameFromLevelTypeAndFileId(int nLevelType, int nFile, char* szFile)
{
	D2LvlTypesTxt* pLevelTypesTxtRecord = NULL;

	D2_ASSERT(sgptDataTables->pLvlTypesTxt);

	pLevelTypesTxtRecord = &sgptDataTables->pLvlTypesTxt[nLevelType];

	*szFile = 0;

	if (pLevelTypesTxtRecord && nFile < 32)
	{
		strcpy_s(szFile, sizeof(pLevelTypesTxtRecord->szFile[nFile]), pLevelTypesTxtRecord->szFile[nFile]);
	}
}

//D2Common.0x6FD61570
void __fastcall DATATBLS_LoadLvlPrestTxt(void* pMemPool, int a2)
{
	int nLength = 0;

	char szBuffer[60] = {};

	D2BinFieldStrc pTbl[] =
	{
		{ "Def", TXTFIELD_DWORD, 0, 0, NULL },
		{ "LevelId", TXTFIELD_DWORD, 0, 4, NULL },
		{ "Populate", TXTFIELD_DWORD, 0, 8, NULL },
		{ "Logicals", TXTFIELD_DWORD, 0, 12, NULL },
		{ "Outdoors", TXTFIELD_DWORD, 0, 16, NULL },
		{ "Animate", TXTFIELD_DWORD, 0, 20, NULL },
		{ "KillEdge", TXTFIELD_DWORD, 0, 24, NULL },
		{ "FillBlanks", TXTFIELD_DWORD, 0, 28, NULL },
		{ "SizeX", TXTFIELD_DWORD, 0, 40, NULL },
		{ "SizeY", TXTFIELD_DWORD, 0, 44, NULL },
		{ "AutoMap", TXTFIELD_DWORD, 0, 48, NULL },
		{ "Scan", TXTFIELD_DWORD, 0, 52, NULL },
		{ "Pops", TXTFIELD_DWORD, 0, 56, NULL },
		{ "PopPad", TXTFIELD_DWORD, 0, 60, NULL },
		{ "Files", TXTFIELD_DWORD, 0, 64, NULL },
		{ "File1", TXTFIELD_ASCII, 59, 68, NULL },
		{ "File2", TXTFIELD_ASCII, 59, 128, NULL },
		{ "File3", TXTFIELD_ASCII, 59, 188, NULL },
		{ "File4", TXTFIELD_ASCII, 59, 248, NULL },
		{ "File5", TXTFIELD_ASCII, 59, 308, NULL },
		{ "File6", TXTFIELD_ASCII, 59, 368, NULL },
		{ "Dt1Mask", TXTFIELD_DWORD, 0, 428, NULL },
		{ "Expansion", TXTFIELD_DWORD, 0, 32, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pLvlPrestTxt = (D2LvlPrestTxt*)DATATBLS_CompileTxt(pMemPool, "lvlprest", pTbl, &sgptDataTables->nLvlPrestTxtRecordCount, sizeof(D2LvlPrestTxt));

	if (a2)
	{
		sgptDataTables->ppLvlPrestFiles = (D2DrlgFileStrc**)D2_CALLOC(6 * sizeof(D2DrlgFileStrc*) * sgptDataTables->nLvlPrestTxtRecordCount);
	}

	for (int i = 0; i < sgptDataTables->nLvlPrestTxtRecordCount; ++i)
	{
		if (FOG_IsExpansion() || !sgptDataTables->pLvlPrestTxt[i].dwExpansion)
		{
			for (int j = 0; j < 6; ++j)
			{
				nLength = strlen(sgptDataTables->pLvlPrestTxt[i].szFile[j]);
				for (int k = 0; k < nLength; ++k)
				{
					if (sgptDataTables->pLvlPrestTxt[i].szFile[j][k] == '/')
					{
						sgptDataTables->pLvlPrestTxt[i].szFile[j][k] = '\\';
					}
				}

				if (nLength > 1)
				{
					wsprintfA(szBuffer, "%s\\%s", "DATA\\GLOBAL\\TILES", sgptDataTables->pLvlPrestTxt[i].szFile[j]);
					strcpy_s(sgptDataTables->pLvlPrestTxt[i].szFile[j], szBuffer);
				}

				if (a2)
				{
					if (nLength > 1)
					{
						DRLGPRESET_LoadDrlgFile(&sgptDataTables->ppLvlPrestFiles[j + 6 * i], NULL, sgptDataTables->pLvlPrestTxt[i].szFile[j]);
					}
				}
			}
		}
	}
}

//D2Common.0x6FD61AD0
void __fastcall DATATBLS_UnloadLvlPrestTxt()
{
	D2DrlgFileStrc** ppLvlPrestFiles = NULL;

	ppLvlPrestFiles = sgptDataTables->ppLvlPrestFiles;
	if (sgptDataTables->ppLvlPrestFiles)
	{
		for (int i = 0; i < 6 * sgptDataTables->nLvlPrestTxtRecordCount; ++i)
		{
			if (ppLvlPrestFiles[i])
			{
				DRLGPRESET_FreeDrlgFile(&ppLvlPrestFiles[i]);
				ppLvlPrestFiles = sgptDataTables->ppLvlPrestFiles;
			}
		}

		D2_FREE(ppLvlPrestFiles);
		sgptDataTables->ppLvlPrestFiles = NULL;
	}

	if (sgptDataTables->pLvlPrestTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pLvlPrestTxt);
	}
}

//D2Common.0x6FD61B50 (#10024)
D2LvlPrestTxt* __stdcall DATATBLS_GetLvlPrestTxtRecord(int nId)
{
	if (nId >= 0 && nId < sgptDataTables->nLvlPrestTxtRecordCount)
	{
		return &sgptDataTables->pLvlPrestTxt[nId];
	}
	
	return NULL;
}

//D2Common.0x6FD61B80
D2LvlPrestTxt* __fastcall DATATBLS_GetLvlPrestTxtRecordFromLevelId(int nLevelId)
{
	for (int i = 0; i < sgptDataTables->nLvlPrestTxtRecordCount; ++i)
	{
		if (sgptDataTables->pLvlPrestTxt[i].dwLevelId == nLevelId)
		{
			return &sgptDataTables->pLvlPrestTxt[i];
		}
	}

	return NULL;
}

//D2Common.0x6FD61BC0
void __fastcall DATATBLS_LoadLvlWarpTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Id", TXTFIELD_DWORD, 0, 0, NULL },
		{ "SelectX", TXTFIELD_DWORD, 0, 4, NULL },
		{ "SelectY", TXTFIELD_DWORD, 0, 8, NULL },
		{ "SelectDX", TXTFIELD_DWORD, 0, 12, NULL },
		{ "SelectDY", TXTFIELD_DWORD, 0, 16, NULL },
		{ "ExitWalkX", TXTFIELD_DWORD, 0, 20, NULL },
		{ "ExitWalkY", TXTFIELD_DWORD, 0, 24, NULL },
		{ "OffsetX", TXTFIELD_DWORD, 0, 28, NULL },
		{ "OffsetY", TXTFIELD_DWORD, 0, 32, NULL },
		{ "LitVersion", TXTFIELD_DWORD, 0, 36, NULL },
		{ "Tiles", TXTFIELD_DWORD, 0, 40, NULL },
		{ "Direction", TXTFIELD_ASCII, 1, 44, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pLvlWarpTxt = (D2LvlWarpTxt*)DATATBLS_CompileTxt(pMemPool, "lvlwarp", pTbl, &sgptDataTables->nLvlWarpTxtRecordCount, sizeof(D2LvlWarpTxt));
}

//D2Common.0x6FD61DD0
void __fastcall DATATBLS_UnloadLvlWarpTxt()
{
	DATATBLS_UnloadBin(sgptDataTables->pLvlWarpTxt);
}

//D2Common.0x6FD61DE0
D2LvlWarpTxt* __fastcall DATATBLS_GetLvlWarpTxtRecordFromLevelIdAndDirection(int nLevelId, char szDirection)
{
	D2_ASSERT(sgptDataTables->pLvlWarpTxt);

	for (int i = 0; i < sgptDataTables->nLvlWarpTxtRecordCount; ++i)
	{
		if (sgptDataTables->pLvlWarpTxt[i].dwLevelId == nLevelId)
		{
			if (szDirection == 'b' || sgptDataTables->pLvlWarpTxt[i].szDirection[0] == 'b' || sgptDataTables->pLvlWarpTxt[i].szDirection[0] == szDirection)
			{
				return &sgptDataTables->pLvlWarpTxt[i];
			}
		}
	}

	FOG_DisplayAssert("Error in DRLG related to stairs that connect levels", __FILE__, __LINE__);
	exit(-1);
}

//D2Common.0x6FD61E60
void __fastcall DATATBLS_LoadLvlMazeTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Level", TXTFIELD_DWORD, 0, 0, NULL },
		{ "Rooms", TXTFIELD_DWORD, 0, 4, NULL },
		{ "Rooms(N)", TXTFIELD_DWORD, 0, 8, NULL },
		{ "Rooms(H)", TXTFIELD_DWORD, 0, 12, NULL },
		{ "SizeX", TXTFIELD_DWORD, 0, 16, NULL },
		{ "SizeY", TXTFIELD_DWORD, 0, 20, NULL },
		{ "Merge", TXTFIELD_DWORD, 0, 24, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pLvlMazeTxt = (D2LvlMazeTxt*)DATATBLS_CompileTxt(pMemPool, "lvlmaze", pTbl, &sgptDataTables->nLvlMazeTxtRecordCount, sizeof(D2LvlMazeTxt));
}

//D2Common.0x6FD61F90
void __fastcall DATATBLS_UnloadLvlMazeTxt()
{
	DATATBLS_UnloadBin(sgptDataTables->pLvlMazeTxt);
}

//D2Common.0x6FD61FA0
D2LvlMazeTxt* __fastcall DATATBLS_GetLvlMazeTxtRecordFromLevelId(int nLevelId)
{
	D2_ASSERT(sgptDataTables->pLvlMazeTxt);
	D2_ASSERT(sgptDataTables->nLvlMazeTxtRecordCount > 0);

	for (int i = 0; i < sgptDataTables->nLvlMazeTxtRecordCount; ++i)
	{
		if (sgptDataTables->pLvlMazeTxt[i].dwLevelId == nLevelId)
		{
			return &sgptDataTables->pLvlMazeTxt[i];
		}
	}
	D2_UNREACHABLE;
}

//D2Common.0x6FD62020
void __fastcall DATATBLS_LoadLvlSubTxt(void* pMemPool, int a2, int a3)
{
	int nLength = 0;
	uint32_t dwType = 0;
	char* szFile = NULL;
	char szBuffer[60] = {};

	D2BinFieldStrc pTbl[] =
	{
		{ "Type", TXTFIELD_DWORD, 0, 0, NULL },
		{ "File", TXTFIELD_ASCII, 59, 4, NULL },
		{ "CheckAll", TXTFIELD_DWORD, 0, 64, NULL },
		{ "BordType", TXTFIELD_DWORD, 0, 68, NULL },
		{ "GridSize", TXTFIELD_DWORD, 0, 76, NULL },
		{ "Dt1Mask", TXTFIELD_DWORD, 0, 72, NULL },
		{ "Prob0", TXTFIELD_DWORD, 0, 284, NULL },
		{ "Trials0", TXTFIELD_DWORD, 0, 304, NULL },
		{ "Max0", TXTFIELD_DWORD, 0, 324, NULL },
		{ "Prob1", TXTFIELD_DWORD, 0, 288, NULL },
		{ "Trials1", TXTFIELD_DWORD, 0, 308, NULL },
		{ "Max1", TXTFIELD_DWORD, 0, 328, NULL },
		{ "Prob2", TXTFIELD_DWORD, 0, 292, NULL },
		{ "Trials2", TXTFIELD_DWORD, 0, 312, NULL },
		{ "Max2", TXTFIELD_DWORD, 0, 332, NULL },
		{ "Prob3", TXTFIELD_DWORD, 0, 296, NULL },
		{ "Trials3", TXTFIELD_DWORD, 0, 316, NULL },
		{ "Max3", TXTFIELD_DWORD, 0, 336, NULL },
		{ "Prob4", TXTFIELD_DWORD, 0, 300, NULL },
		{ "Trials4", TXTFIELD_DWORD, 0, 320, NULL },
		{ "Max4", TXTFIELD_DWORD, 0, 340, NULL },
		{ "Expansion", TXTFIELD_DWORD, 0, 344, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pLvlSubTxt = (D2LvlSubTxt*)DATATBLS_CompileTxt(pMemPool, "lvlsub", pTbl, &sgptDataTables->nLvlSubTxtRecordCount, sizeof(D2LvlSubTxt));

	if (sgptDataTables->nLvlSubTxtRecordCount > 0)
	{
		dwType = 0;
		for (int i = 0; i < sgptDataTables->nLvlSubTxtRecordCount; ++i)
		{
			if (sgptDataTables->pLvlSubTxt[i].dwType > dwType)
			{
				dwType = sgptDataTables->pLvlSubTxt[i].dwType;
			}
		}

		if (dwType)
		{
			sgptDataTables->pLvlSubTypeStartIds = (int*)D2_CALLOC_POOL(nullptr, sizeof(int) * (dwType + 1));

			dwType = 0;
			for (int i = 0; i < sgptDataTables->nLvlSubTxtRecordCount; ++i)
			{
				if (sgptDataTables->pLvlSubTxt[i].dwType != dwType)
				{
					dwType = sgptDataTables->pLvlSubTxt[i].dwType;
					sgptDataTables->pLvlSubTypeStartIds[dwType] = i;
				}
			}
		}
	}

	if (!a2)
	{
		D2_ASSERT(gpLvlSubTypeFilesCriticalSection == NULL);
		gpLvlSubTypeFilesCriticalSection = D2_ALLOC_STRC(CRITICAL_SECTION);
		InitializeCriticalSection(gpLvlSubTypeFilesCriticalSection);
	}

	if (a2 || a3)
	{
		sgptDataTables->ppLvlSubTypeFiles = (D2DrlgFileStrc**)D2_CALLOC(sizeof(D2DrlgFileStrc*) * sgptDataTables->nLvlSubTxtRecordCount);
	}


	for (int i = 0; i < sgptDataTables->nLvlSubTxtRecordCount; ++i)
	{
		szFile = sgptDataTables->pLvlSubTxt[i].szFile;

		nLength = strlen(szFile);
		for (int j = 0; j < nLength; ++j)
		{
			if (szFile[j] == '/')
			{
				szFile[j] = '\\';
			}
		}

		if (nLength > 1)
		{
			wsprintfA(szBuffer, "%s\\%s", "DATA\\GLOBAL\\TILES", szFile);
			strcpy_s(sgptDataTables->pLvlSubTxt[i].szFile, szBuffer);
		}

		if ((FOG_IsExpansion() || !sgptDataTables->pLvlSubTxt[i].dwExpansion) && (a2 || a3))
		{
			DRLGPRESET_LoadDrlgFile(&sgptDataTables->ppLvlSubTypeFiles[i], NULL, szFile);
			D2_ASSERT(sgptDataTables->ppLvlSubTypeFiles[i]->nSubstGroups);
		}
	}
}

//D2Common.0x6FD62600
void __fastcall DATATBLS_UnloadLvlSubTxt()
{
	D2DrlgFileStrc** ppFiles = sgptDataTables->ppLvlSubTypeFiles;

	if (sgptDataTables->ppLvlSubTypeFiles)
	{
		for (int i = 0; i < sgptDataTables->nLvlSubTxtRecordCount; ++i)
		{
			if (ppFiles[i])
			{
				DRLGPRESET_FreeDrlgFile(&ppFiles[i]);
				ppFiles = sgptDataTables->ppLvlSubTypeFiles;
			}
		}

		D2_FREE(ppFiles);
		sgptDataTables->ppLvlSubTypeFiles = NULL;
	}

	if (sgptDataTables->pLvlSubTxt)
	{
		for (int i = 0; i < sgptDataTables->nLvlSubTxtRecordCount; ++i)
		{
			DRLGTILESUB_FreeDrlgFile(&sgptDataTables->pLvlSubTxt[i]);
		}

		DATATBLS_UnloadBin(sgptDataTables->pLvlSubTxt);
		sgptDataTables->pLvlSubTxt = NULL;
	}

	if (sgptDataTables->pLvlSubTypeStartIds)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pLvlSubTypeStartIds);
		sgptDataTables->pLvlSubTypeStartIds = NULL;
	}

	if (gpLvlSubTypeFilesCriticalSection)
	{
		DeleteCriticalSection(gpLvlSubTypeFilesCriticalSection);
		D2_FREE(gpLvlSubTypeFilesCriticalSection);
		gpLvlSubTypeFilesCriticalSection = NULL;
	}
}

//D2Common.0x6FD626F0
D2LvlSubTxt* __fastcall DATATBLS_GetLvlSubTxtRecord(int nSubType)
{
	D2_ASSERT(nSubType >= 0);
	D2_ASSERT(sgptDataTables->pLvlSubTxt);
	D2_ASSERT(sgptDataTables->pLvlSubTypeStartIds);

	return &sgptDataTables->pLvlSubTxt[sgptDataTables->pLvlSubTypeStartIds[nSubType]];
}

//D2Common.0x6FD62780
void __fastcall DATATBLS_AllocGlobalTileLibraryHash()
{
	static char szTest[] = "0";

	D2TileLibraryHashStrc** ppTileLibraryHash = NULL;
	bool bValidFileName = false;

	D2_ASSERT(sgptDataTables->pLvlTypesTxt);

	sgptDataTables->ppTileLibraryHash = (D2TileLibraryHashStrc**)D2_CALLOC_POOL(nullptr, sizeof(D2TileLibraryHashStrc*[1024]) * sgptDataTables->nLvlTypesTxtRecordCount);
	
	ppTileLibraryHash = sgptDataTables->ppTileLibraryHash;
	for (int i = 0; i < sgptDataTables->nLvlTypesTxtRecordCount; ++i)
	{
		for (int j = 0; j < 32; ++j)
		{
			bValidFileName = false;
			for (int k = 0; k < ARRAY_SIZE(szTest); ++k)
			{
				bValidFileName = sgptDataTables->pLvlTypesTxt[i].szFile[j][k] != szTest[k];
				if (bValidFileName)
				{
					break;
				}
			}
			if (!bValidFileName)
			{
				break;
			}

			if (FOG_IsExpansion() || !sgptDataTables->pLvlTypesTxt[i].dwExpansion)
			{
				D2CMP_10087_LoadTileLibrarySlot(ppTileLibraryHash, sgptDataTables->pLvlTypesTxt[i].szFile[j]);
			}
		}

		ppTileLibraryHash += 1024;
	}
}

//D2Common.0x6FD628A0
void __fastcall DATATBLS_FreeGlobalTileLibraryHash()
{
	if (sgptDataTables->ppTileLibraryHash)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->ppTileLibraryHash);
	}
}

//D2Common.0x6FD628C0
void __fastcall DATATBLS_LoadAutomapTxt(void* pMemPool)
{
	D2AutomapShortStrc* pAutomap = NULL;
	D2AutomapTxt* pAutomapTxt = NULL;
	unsigned int nAutomapLevelType = 0;
	unsigned int nLevelCounter = 0;
	unsigned int nTileCounter = 0;
	int nAutomapTxtRecordCount = 0;
	int nCounter = 0;

	D2BinFieldStrc pTbl[] =
	{
		{ "LevelName", TXTFIELD_ASCII, 15, 0, NULL },
		{ "TileName", TXTFIELD_ASCII, 7, 16, NULL },
		{ "Style", TXTFIELD_BYTE, 0, 24, NULL },
		{ "StartSequence", TXTFIELD_BYTE, 0, 25, NULL },
		{ "EndSequence", TXTFIELD_BYTE, 0, 26, NULL },
		{ "Cel1", TXTFIELD_DWORD, 0, 28, NULL },
		{ "Cel2", TXTFIELD_DWORD, 0, 32, NULL },
		{ "Cel3", TXTFIELD_DWORD, 0, 36, NULL },
		{ "Cel4", TXTFIELD_DWORD, 0, 40, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	SEED_InitSeed(gpAutomapSeed);
	SEED_InitLowSeed(gpAutomapSeed, 0);

	pAutomapTxt = (D2AutomapTxt*)DATATBLS_CompileTxt(pMemPool, "automap", pTbl, &nAutomapTxtRecordCount, sizeof(D2AutomapTxt));

	pAutomap = (D2AutomapShortStrc*)D2_CALLOC_POOL(nullptr, sizeof(D2AutomapShortStrc) * nAutomapTxtRecordCount);

	for (int i = 0; i < nAutomapTxtRecordCount; ++i)
	{
		nLevelCounter = 0;
		while (0 != strcmp(pAutomapTxt[i].szLevelName, gszAutomapLevelNames[nLevelCounter]))
		{
			++nLevelCounter;
			D2_ASSERT(nLevelCounter < ARRAY_SIZE(gszAutomapLevelNames));
		}
		pAutomap[i].dwAutomapLevelType = nLevelCounter;

		nTileCounter = 0;
		while (0 != strcmp(pAutomapTxt[i].szTileName, gszAutomapTileNames[nTileCounter]))
		{
			++nTileCounter;
			D2_ASSERT(nTileCounter < ARRAY_SIZE(gszAutomapTileNames));
		}
		pAutomap[i].dwAutomapTileType = nTileCounter;

		pAutomap[i].nStyle = pAutomapTxt[i].nStyle;
		pAutomap[i].nStartSequence = pAutomapTxt[i].nStartSequence;
		pAutomap[i].nEndSequence = pAutomapTxt[i].nEndSequence;
		pAutomap[i].nCel[0] = pAutomapTxt[i].nCel[0];
		pAutomap[i].nCel[1] = pAutomapTxt[i].nCel[1];
		pAutomap[i].nCel[2] = pAutomapTxt[i].nCel[2];
		pAutomap[i].nCel[3] = pAutomapTxt[i].nCel[3];
		pAutomap[i].nCels = 0;

		for (int j = 0; j < 4; ++j)
		{
			if (pAutomap[i].nCel[j] == -1)
			{
				break;
			}
			++pAutomap[i].nCels;
		}

		D2_ASSERT(pAutomap[i].nCels);
	}

	DATATBLS_UnloadBin(pAutomapTxt);

	sgptDataTables->pAutoMap = pAutomap;
	sgptDataTables->nAutoMap = nAutomapTxtRecordCount;

	memset(sgptDataTables->pAutoMapRand, -1, sizeof(sgptDataTables->pAutoMapRand));

	nCounter = 0;
	while (nCounter < nAutomapTxtRecordCount)
	{
		nAutomapLevelType = pAutomap[nCounter].dwAutomapLevelType;

		sgptDataTables->pAutoMapRand[nAutomapLevelType].nStart = nCounter;

		while (nCounter + 1 < nAutomapTxtRecordCount)
		{
			if (nAutomapLevelType != pAutomap[nCounter + 1].dwAutomapLevelType)
			{
				break;
			}

			++nCounter;
		}

		++nCounter;

		sgptDataTables->pAutoMapRand[nAutomapLevelType].nEnd = nCounter;
	}
}

//D2Common.0x6FD62D30 (#10011)
int __fastcall DATATBLS_GetAutomapCellId(uint32_t dwAutomapLevelType, uint32_t dwAutomapTileType, int nStyle, int nSequence)
{
	int nStart = 0;
	int nEnd = 0;
	int nCels = 0;
	int i = 0;

	nStart = sgptDataTables->pAutoMapRand[dwAutomapLevelType].nStart;
	nEnd = sgptDataTables->pAutoMapRand[dwAutomapLevelType].nEnd;

	if (!dwAutomapLevelType || nStart == -1)
	{
		return -1;
	}

	D2_ASSERT(nEnd != -1);

	if (nStart < nEnd)
	{
		i = nStart;
		while (sgptDataTables->pAutoMap[i].dwAutomapLevelType != dwAutomapLevelType
			   || sgptDataTables->pAutoMap[i].dwAutomapTileType != dwAutomapTileType
			   || sgptDataTables->pAutoMap[i].nStyle != -1 && sgptDataTables->pAutoMap[i].nStyle != nStyle
			   || sgptDataTables->pAutoMap[i].nStartSequence != -1
			   && (sgptDataTables->pAutoMap[i].nStartSequence > nSequence || sgptDataTables->pAutoMap[i].nEndSequence < nSequence))
		{
			++i;
			if (i >= nEnd)
			{
				return -1;
			}
		}

		nCels = sgptDataTables->pAutoMap[i].nCels;
		if (nCels > 0)
		{
			return sgptDataTables->pAutoMap->nCel[SEED_RollLimitedRandomNumber(gpAutomapSeed, nCels) + 8 * i];
		}
		else
		{
			return sgptDataTables->pAutoMap[i].nCel[0];
		}
	}

	return -1;
}

//D2Common.0x6FD62E70
void __fastcall DATATBLS_FreeAutomap()
{
	if (sgptDataTables->pAutoMap)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pAutoMap);
		sgptDataTables->pAutoMap = NULL;
	}
}
