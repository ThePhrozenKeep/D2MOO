#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <D2DataTbls.h>

#include <Drlg/D2DrlgActivate.h>
#include <Drlg/D2DrlgPreset.h>
#include <Drlg/D2DrlgOutPlace.h>
#include <Drlg/D2DrlgTileSub.h>
#include <Drlg/D2DrlgDrlgWarp.h>
#include <Drlg/D2DrlgDrlgLogic.h>
#include <Drlg/D2DrlgMaze.h>
#include <D2StatList.h>
#include <Units/Units.h>
#include <DataTbls/MonsterTbls.h>
#include <vector>
#include <detours.h>


static HMODULE OriginalD2Common = LoadLibraryA("D2Common.dll");
static const int D2CommonImageBase = 0x6FD40000;

#define DEFINE_ORGINAL_D2COMMON_DLL_PTR(function, offset) \
using function##TypePtr = decltype(function)*; \
static const function##TypePtr Original_##function = (function##TypePtr)(uintptr_t(OriginalD2Common) + (offset - D2CommonImageBase))

DEFINE_ORGINAL_D2COMMON_DLL_PTR(DRLGPRESET_ParseDS1File, 0x6FD85A10);
DEFINE_ORGINAL_D2COMMON_DLL_PTR(D2Common_10474, 0x6FDB91C0);
DEFINE_ORGINAL_D2COMMON_DLL_PTR(DATATBLS_LoadAllTxts, 0x6FD504B0);
DEFINE_ORGINAL_D2COMMON_DLL_PTR(DRLG_AllocDrlg, 0x6FD74120);
DEFINE_ORGINAL_D2COMMON_DLL_PTR(DRLG_FreeDrlg, 0x6FD743B0);

#include <filesystem>

bool operator==(const D2MapAIStrc& lhs, const D2MapAIStrc& rhs)
{
	if (lhs.nPathNodes != rhs.nPathNodes)
		return false;
	for (int i = 0; i < lhs.nPathNodes; i++)
	{
		if (lhs.pPosition[i].nX != rhs.pPosition[i].nX) return false;
		if (lhs.pPosition[i].nY != rhs.pPosition[i].nY) return false;
		if (lhs.pPosition[i].nMapAIAction != rhs.pPosition[i].nMapAIAction) return false;
	}
	return true;
}
bool operator==(const D2PresetUnitStrc& lhs, const D2PresetUnitStrc& rhs)
{
	return true
		&& lhs.nUnitType == rhs.nUnitType
		&& lhs.nMode == rhs.nMode
		&& lhs.nIndex == rhs.nIndex
		&& lhs.nXpos == rhs.nXpos
		&& lhs.nYpos == rhs.nYpos
		&& lhs.bSpawned == rhs.bSpawned
		&& ((!lhs.pMapAI && !rhs.pMapAI) || (*lhs.pMapAI == *rhs.pMapAI))
		;
}

void __fastcall DetouredFog_10251_ValidateCriticalSection(LPCRITICAL_SECTION lp, int a2)
{
	// Do nothing instead of checking internals and then crash...
}
void DetouredFog_10029_PostToDebugFile(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);
	vprintf(format, vl);
	putc('\n',stdout);
	va_end(vl);
}
void PatchFogDll()
{
	static bool needsPatching = true;
	if (!needsPatching)return;
	if (NO_ERROR == DetourTransactionBegin())
	{
		DetourUpdateThread(GetCurrentThread());
		bool success = true;

		// Somehow Fog.dll in v1.10 will check critical sections validity... using internal structures.
		// Those internal structures obviously changed since then, so the check will fail. (ABI was broken)
		// It will then try to assert, check the validity of the CS again, and recurse blowing up the stack.
		// This check should never have made it into a release build anyway, so remove it.
		HMODULE fogHandle = GetModuleHandleA("Fog.dll");
		REQUIRE(fogHandle != NULL);
		void* TrueFog_10251 = GetProcAddress(fogHandle, (LPCSTR)10251);
		REQUIRE(TrueFog_10251);
		void* TrueFog_10029_PostToDebugFile = GetProcAddress(fogHandle, (LPCSTR)10029);
		REQUIRE(TrueFog_10029_PostToDebugFile);
		success = success && NO_ERROR == DetourAttach(&(PVOID&)TrueFog_10251, DetouredFog_10251_ValidateCriticalSection);
		success = success && NO_ERROR == DetourAttach(&(PVOID&)TrueFog_10029_PostToDebugFile, DetouredFog_10029_PostToDebugFile);
		success = NO_ERROR == DetourTransactionCommit();
		REQUIRE(success);
	}

	needsPatching = false;
}

void PatchD2Common()
{
	if (NO_ERROR == DetourTransactionBegin())
	{
		DetourUpdateThread(GetCurrentThread());
		bool success = true;

		// Use the original game version

#define REPLACE_D2MOO_BY_ORG_COMMON(func, address) \
		void* D2MOO_##func = func;        \
		success = success && NO_ERROR == DetourAttach(&(PVOID&)D2MOO_##func, (void*)(uintptr_t(OriginalD2Common) + (address - D2CommonImageBase)))

		
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLG_InitLevel, 0x6FD74C10);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGMAZE_GenerateLevel, 0x6FD794A0);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGPRESET_GenerateLevel, 0x6FD886F0);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLG_ComputeLevelWarpInfo, 0x6FD74B40);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGWARP_GetWarpDestinationFromArray, 0x6FD78D10);
		
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGOUTPLACE_CreateLevelConnections, 0x6FD81D60);
		//REPLACE_D2MOO_BY_ORG_COMMON(sub_6FD73E30, 0x6FD73E30);

		//REPLACE_D2MOO_BY_ORG_COMMON(sub_6FD823C0, 0x6FD823C0);
		//REPLACE_D2MOO_BY_ORG_COMMON(sub_6FD82750, 0x6FD82750);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGOUTPLACE_LinkAct2Outdoors, 0x6FD821E0);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGOUTPLACE_LinkAct2Canyon, 0x6FD82240);
		//REPLACE_D2MOO_BY_ORG_COMMON(sub_6FD81330, 0x6FD81330);
		//REPLACE_D2MOO_BY_ORG_COMMON(sub_6FD81B30, 0x6FD81B30);
		//REPLACE_D2MOO_BY_ORG_COMMON(sub_6FD81530, 0x6FD81530);
		//REPLACE_D2MOO_BY_ORG_COMMON(sub_6FD81BF0, 0x6FD81BF0);
		
		void* Original_DRLGPRESET_ParseDS1File = (void*)(uintptr_t(OriginalD2Common) + (0x6FD85A10 - D2CommonImageBase));
		success = success && NO_ERROR == DetourAttach(&(PVOID&)Original_DRLGPRESET_ParseDS1File, DRLGPRESET_ParseDS1File);

		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGPRESET_BuildArea, 0x6FD87560);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGGRID_FillGrid, 0x6FD762B0);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGPRESET_BuildPresetArea, 0x6FD87760);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGPRESET_InitPresetRoomData, 0x6FD86DC0);
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLGGRID_GetGridFlags, 0x6FD75CA0);
		
		//REPLACE_D2MOO_BY_ORG_COMMON(DRLG_GenerateJungles, 0x6FD82820);

		success = NO_ERROR == DetourTransactionCommit();
		REQUIRE(success);
	}
}

void* (__fastcall *D2Lang_10000_LoadStringTables)(int a1, const char* a2, BOOL bLoadExpansion);

void InitGameIfNeeded()
{
	static bool needsInit = true;
	if (!needsInit)return;

	PatchFogDll();
	PatchD2Common();

	sgptDataTables = *(D2DataTablesStrc**)(uintptr_t(OriginalD2Common) + (0x6FDD6A20 - D2CommonImageBase));


	// We want to use the files in the workind directory, no archive
	FOG_10021("D2");                        //D2InitLogManager
	//FOG_InitErrorMgr("D2 Tests", []() -> int { REQUIRE(false); return TRUE; }, "v0.0.1", false);
	char versionStr[MAX_PATH];
	strncpy(versionStr, "v1.10", sizeof versionStr);
	FOG_InitErrorMgr("Diablo II", NULL, versionStr, TRUE);    //D2SystemInit

	// Note: semble insuffisant pour l'extension, ou alors il y a un problème ailleurs.
	// On essaie de charger data\global\tiles\expansion\Town\townWest.ds1 avec le path expansion/Town/townWest.ds1.
	FOG_MPQSetConfig(true, true);

	HMODULE langHandle = GetModuleHandle("D2Lang.dll");
	REQUIRE(langHandle);
	D2Lang_10000_LoadStringTables = (decltype(D2Lang_10000_LoadStringTables))GetProcAddress(langHandle, (LPCSTR)10000);
	REQUIRE(D2Lang_10000_LoadStringTables);
	D2Lang_10000_LoadStringTables(0, "FRA", FALSE);


	Original_DATATBLS_LoadAllTxts(nullptr, true, true);

	needsInit = false;
}

void checkParseDS1(const char* filePath)
{	
	InitGameIfNeeded();

	CAPTURE(filePath);

	D2DrlgFileStrc drlgFile{};
	DRLGPRESET_ParseDS1File(&drlgFile, nullptr, filePath);

	D2DrlgFileStrc drlgFileOriginal{};
	Original_DRLGPRESET_ParseDS1File(&drlgFileOriginal, nullptr, filePath);

	CHECK(drlgFile.unk0x00 == drlgFileOriginal.unk0x00);
	CHECK(drlgFile.unk0x08 == drlgFileOriginal.unk0x08);
	CHECK(drlgFile.nWidth == drlgFileOriginal.nWidth);
	CHECK(drlgFile.nHeight == drlgFileOriginal.nHeight);
	CHECK(drlgFile.nWalls == drlgFileOriginal.nWalls);
	CHECK(drlgFile.nFloors == drlgFileOriginal.nFloors);
	CHECK(drlgFile.nSubstGroups == drlgFileOriginal.nSubstGroups);

	int nArea = (drlgFile.nWidth + 1) * (drlgFile.nHeight + 1);

	REQUIRE(drlgFile.nFloors <= 4);
	for (int i = 0; i < drlgFile.nWalls; i++)
	{
		CHECK(drlgFile.pOrientationLayer[i] != nullptr);
		CHECK(drlgFileOriginal.pOrientationLayer[i] != nullptr);
		CHECK(0 == memcmp(drlgFile.pOrientationLayer[i], drlgFileOriginal.pOrientationLayer[i], nArea * sizeof(uint32_t)));

		CHECK(drlgFile.pWallLayer[i] != nullptr);
		CHECK(drlgFileOriginal.pWallLayer[i] != nullptr);
		CHECK(0 == memcmp(drlgFile.pWallLayer[i], drlgFileOriginal.pWallLayer[i], nArea * sizeof(uint32_t)));
	}

	REQUIRE(drlgFile.nFloors <= 2);
	for (int i = 0; i < drlgFile.nFloors; i++)
	{
		CHECK(drlgFile.pFloorLayer[i] != nullptr);
		CHECK(drlgFileOriginal.pFloorLayer[i] != nullptr);
		CHECK(0 == memcmp(drlgFile.pFloorLayer[i], drlgFileOriginal.pFloorLayer[i], nArea * sizeof(uint32_t)));
	}

	CHECK(drlgFile.pShadowLayer != nullptr);
	CHECK(drlgFileOriginal.pShadowLayer != nullptr);
	CHECK(0 == memcmp(drlgFile.pShadowLayer, drlgFileOriginal.pShadowLayer, nArea * sizeof(uint32_t)));

	CHECK_UNARY(drlgFile.pSubstGroups != nullptr || drlgFile.nSubstGroups == 0);
	CHECK_UNARY(drlgFileOriginal.pSubstGroups != nullptr || drlgFileOriginal.nSubstGroups == 0);
	for (int i = 0; i < drlgFile.nSubstGroups; ++i)
	{
		CHECK(drlgFile.pSubstGroups[i].field_0 == drlgFileOriginal.pSubstGroups[i].field_0);
		CHECK(drlgFile.pSubstGroups[i].field_4 == drlgFileOriginal.pSubstGroups[i].field_4);
		CHECK(drlgFile.pSubstGroups[i].field_8 == drlgFileOriginal.pSubstGroups[i].field_8);
		CHECK(drlgFile.pSubstGroups[i].field_C == drlgFileOriginal.pSubstGroups[i].field_C);
		// +0x10 uninitialized ?
		WARN(drlgFile.pSubstGroups[i].field_10 == drlgFileOriginal.pSubstGroups[i].field_10);
		CHECK(drlgFile.pSubstGroups[i].field_14 == drlgFileOriginal.pSubstGroups[i].field_14);
	}

	D2PresetUnitStrc* pPresetUnit = drlgFile.pPresetUnit;
	D2PresetUnitStrc* pPresetUnitOriginal = drlgFileOriginal.pPresetUnit;
	int i = 0;
	while (pPresetUnit && pPresetUnitOriginal)
	{
		CAPTURE(i);
		CHECK(*pPresetUnit == *pPresetUnitOriginal);
		i++;
		pPresetUnit = pPresetUnit->pNext;
		pPresetUnitOriginal = pPresetUnitOriginal->pNext;
	}
}

TEST_CASE("ParseDS1")
{
	for (auto& p : std::filesystem::recursive_directory_iterator(R"(DATA\GLOBAL\TILES\)"))
	{
		if (p.is_regular_file() && p.path().extension() == ".ds1")
		{
			checkParseDS1(p.path().string().c_str());
		}
	}
}


void CheckSameContent(const D2CoordStrc& c1, const D2CoordStrc& c2)
{
	CHECK(c1.nX == c2.nX);
	CHECK(c1.nY == c2.nY);
}

void CheckSameContent(const D2DrlgCoordStrc& c1, const D2DrlgCoordStrc& c2)
{
	CHECK(c1.nPosX == c2.nPosX);
	CHECK(c1.nPosY == c2.nPosY);
	CHECK(c1.nWidth == c2.nWidth);
	CHECK(c1.nHeight == c2.nHeight);
}

void CheckSameContent(const D2DrlgGridStrc& grid1, const D2DrlgGridStrc& grid2)
{
	CHECK(grid1.nWidth == grid2.nWidth);
	CHECK(grid1.nHeight == grid2.nHeight);
	for (int nY = 0; nY < grid1.nHeight; nY++)
	{
		REQUIRE(grid1.pCellsRowOffsets[nY] == grid2.pCellsRowOffsets[nY]);
		for (int nX = 0; nX < grid1.nWidth; nX++)
		{
			CHECK(grid1.pCellsFlags[nX + grid1.pCellsRowOffsets[nY]] == grid2.pCellsFlags[nX + grid2.pCellsRowOffsets[nY]]);
		}
	}
}

void CheckSameContent(const D2DrlgMapStrc* map1, const D2DrlgMapStrc* map2)
{

	while (map1 && map2)
	{
		CHECK(map1->nLevelPrest == map2->nLevelPrest);
		CHECK(map1->nPickedFile == map2->nPickedFile);


		D2LvlPrestTxt* pLvlPrestTxtRecord;
		D2DrlgFileStrc* pFile;
		CheckSameContent(map1->pDrlgCoord, map2->pDrlgCoord);

		CHECK(map1->bHasInfo == map2->bHasInfo);
		CheckSameContent(map1->pMapGrid, map2->pMapGrid);

		// TODO
		D2PresetUnitStrc* pPresetUnit;

		CHECK(map1->bInited == map2->bInited);
		REQUIRE(map1->nPops == map2->nPops);
		for (int i = 0; i < map1->nPops; i++)
		{
			CHECK(map1->pPopsIndex[i] == map2->pPopsIndex[i]);
			CHECK(map1->pPopsSubIndex[i] == map2->pPopsSubIndex[i]);
			CHECK(map1->pPopsOrientation[i] == map2->pPopsOrientation[i]);
			CheckSameContent(map1->pPopsLocation[i], map2->pPopsLocation[i]);
		}
		CHECK(map1->bHasInfo == map2->bHasInfo);

		map1 = map1->pNext;
		map2 = map2->pNext;
	}
	REQUIRE(!map1);
	REQUIRE(!map2);
}



void CheckSameContent(const D2DrlgPresetRoomStrc* preset1, const D2DrlgPresetRoomStrc* preset2)
{
	REQUIRE(preset1 != nullptr);
	REQUIRE(preset2 != nullptr);
	CHECK(preset1->nLevelPrest == preset2->nLevelPrest);
	CHECK(preset1->nPickedFile == preset2->nPickedFile);
	
	CheckSameContent(preset1->pMap, preset2->pMap);

	CHECK(preset1->dwFlags == preset2->dwFlags);

	for (int i = 0; i < std::size(preset1->pWallGrid); i++)
		CheckSameContent(preset1->pWallGrid[i], preset2->pWallGrid[i]);
	for (int i = 0; i < std::size(preset1->pOrientationGrid); i++)
		CheckSameContent(preset1->pOrientationGrid[i], preset2->pOrientationGrid[i]);
	for (int i = 0; i < std::size(preset1->pFloorGrid); i++)
		CheckSameContent(preset1->pFloorGrid[i], preset2->pFloorGrid[i]);
	CheckSameContent(preset1->pCellGrid, preset2->pCellGrid);
	
	REQUIRE(preset1->nTombStoneTiles == preset2->nTombStoneTiles);
	for(int i=0; i < preset1->nTombStoneTiles; i ++)
		CheckSameContent(preset1->pTombStoneTiles[i], preset2->pTombStoneTiles[i]);


}


void CheckSameContent(const D2RoomCoordListStrc* lhs, const D2RoomCoordListStrc* rhs)
{
	while (lhs && rhs)
	{
		CheckSameContent(lhs->pBox[0], rhs->pBox[0]);
		CheckSameContent(lhs->pBox[1], rhs->pBox[1]);
		CHECK(lhs->bNode       == rhs->bNode);
		CHECK(lhs->bRoomActive == rhs->bRoomActive);
		CHECK(lhs->nIndex      == rhs->nIndex);

		lhs = lhs->pNext;
		rhs = rhs->pNext;
	}
	CHECK(lhs == nullptr);
	CHECK(rhs == nullptr);
}

void CheckSameContent(const D2DrlgCoordListStrc* lhs, const D2DrlgCoordListStrc* rhs)
{
	if (lhs && rhs)
	{
		CHECK(lhs->dwFlags == rhs->dwFlags);
		CHECK(lhs->nLists == rhs->nLists);
		CheckSameContent(lhs->pIndexX, rhs->pIndexX);
		CheckSameContent(lhs->pIndexY, rhs->pIndexY);
		for (int i = 0; i < rhs->nLists; i++)
		{
			CheckSameContent(lhs->pCoordList, rhs->pCoordList);
		}
	}
	else
	{
		CHECK(lhs == nullptr);
		CHECK(rhs == nullptr);
	}
}

void CheckSameContent(const D2DrlgWarpStrc* lhs, const D2DrlgWarpStrc* rhs)
{
	while (lhs && rhs)
	{
		CHECK(lhs->nLevel == rhs->nLevel);
		for (int i = 0; i < sizeof(D2DrlgWarpStrc::nVis) / sizeof(D2DrlgWarpStrc::nVis[0]); i++)
			CHECK(lhs->nVis[i] == rhs->nVis[i]);
		for (int i = 0; i < sizeof(D2DrlgWarpStrc::nWarp) / sizeof(D2DrlgWarpStrc::nWarp[0]); i++)
			CHECK(lhs->nWarp[i] == rhs->nWarp[i]);

		lhs = lhs->pNext;
		rhs = rhs->pNext;
	}
	CHECK(lhs == nullptr);
	CHECK(rhs == nullptr);
}

void CheckContent(const D2RoomExStrc* lhs, const D2RoomExStrc* rhs, bool bCheckStatusNull)
{
	if (!lhs || !rhs)
	{
		CHECK(lhs == nullptr);
		CHECK(rhs == nullptr);
		return;
	}
	CHECK(lhs->pDrlgCoord.nPosX == rhs->pDrlgCoord.nPosX);
	CHECK(lhs->pDrlgCoord.nPosY == rhs->pDrlgCoord.nPosY);
	CHECK(lhs->pDrlgCoord.nWidth == rhs->pDrlgCoord.nWidth);
	CHECK(lhs->pDrlgCoord.nHeight == rhs->pDrlgCoord.nHeight);
	CHECK(lhs->dwFlags == rhs->dwFlags);
	CHECK(lhs->dwOtherFlags == rhs->dwOtherFlags);
	REQUIRE(lhs->nType == rhs->nType);
	switch (lhs->nType)
	{
	case 0:
		break; // Not initialized
	case DRLGTYPE_MAZE:
	case DRLGTYPE_PRESET:
		CheckSameContent(lhs->pMaze, rhs->pMaze);
		break;
	case DRLGTYPE_OUTDOOR:
		break;
	default:REQUIRE(false); break;
	}

	REQUIRE(lhs->dwDT1Mask == rhs->dwDT1Mask);

	for (D2TileLibraryHashStrc* pTile : lhs->pTiles)
		REQUIRE(pTile == nullptr);
	for (D2TileLibraryHashStrc* pTile : rhs->pTiles)
		REQUIRE(pTile == nullptr);


	REQUIRE(lhs->pTileGrid == nullptr);
	REQUIRE(rhs->pTileGrid == nullptr);

	REQUIRE(lhs->fRoomStatus == rhs->fRoomStatus);
	REQUIRE(lhs->unk0xAD == rhs->unk0xAD);
	for (int i = 0; i < 5; i++)
		REQUIRE(lhs->unk0xAE[i] == rhs->unk0xAE[i]);

	if (bCheckStatusNull)
	{
		REQUIRE(lhs->pStatusNext == nullptr);
		REQUIRE(rhs->pStatusNext == nullptr);
		REQUIRE(lhs->pStatusPrev == nullptr);
		REQUIRE(rhs->pStatusPrev == nullptr);
	}

	REQUIRE(lhs->ppRoomsNear == nullptr);
	REQUIRE(rhs->ppRoomsNear == nullptr);

	REQUIRE(lhs->nRoomsNear == rhs->nRoomsNear);

	REQUIRE(lhs->pRoomTiles == nullptr);
	REQUIRE(rhs->pRoomTiles == nullptr);

	REQUIRE(lhs->pPresetUnits == nullptr);
	REQUIRE(rhs->pPresetUnits == nullptr);

	REQUIRE(lhs->pDrlgOrth == nullptr);
	REQUIRE(rhs->pDrlgOrth == nullptr);

	REQUIRE(lhs->pSeed.lSeed == rhs->pSeed.lSeed);
	REQUIRE(lhs->dwInitSeed == rhs->dwInitSeed);

	// pCoordList expected to be nullptr at this point
	CheckSameContent(lhs->pCoordList, rhs->pCoordList);

	REQUIRE(lhs->pRoom == nullptr);
	REQUIRE(rhs->pRoom == nullptr);
}

TEST_CASE("DRLG_AllocDrlg")
{
	const bool checkAllSeeds = true;
	const uint32_t lastSeed = checkAllSeeds ? std::numeric_limits<uint32_t>::max() : 0;

	for (uint32_t seed = 0; seed != lastSeed; seed++)
	{
		CAPTURE(seed);
		for (const D2C_Difficulties nDifficulty : { DIFFMODE_NORMAL, DIFFMODE_NIGHTMARE, DIFFMODE_HELL})
		{
			CAPTURE(nDifficulty);

			const D2C_TownLevels actTownLevelIds[] = { D2TOWN_ROGUECAMP, D2TOWN_LUTGHOL, D2TOWN_KURAST, D2TOWN_PANDFORT, D2TOWN_HARRO };
			for (int nAct = ACT_I; nAct <= ACT_V; nAct++)
			{
				const D2C_TownLevels actTownId = actTownLevelIds[nAct];

				D2DrlgActStrc act{};
				act.nAct = nAct;
				act.dwInitSeed = seed;
				act.nTownId = actTownId;
				D2DrlgActStrc orgAct = act;

				// Looks like it is not actually used in D2Common ?
				D2GameStrc game{};
				game.dwInitSeed = act.dwInitSeed;
				game.bExpansion = true;
				D2GameStrc orgGame = game;


				D2DrlgStrc* pOrgDrlg = Original_DRLG_AllocDrlg(&orgAct, act.nAct, 0, orgAct.dwInitSeed, actTownId, D2DrlgFlags(0), &orgGame, DIFFMODE_NORMAL, nullptr, nullptr);
				D2DrlgStrc* pDrlg = DRLG_AllocDrlg(&act, act.nAct, 0, act.dwInitSeed, actTownId, D2DrlgFlags(0), &game, DIFFMODE_NORMAL, nullptr, nullptr);

				CHECK(pDrlg->unk0x08 == pOrgDrlg->unk0x08);
				// pAct
				CHECK(pDrlg->nAct == pOrgDrlg->nAct);
				// pad0x11
				CHECK(pDrlg->pSeed.lSeed == pOrgDrlg->pSeed.lSeed);
				CHECK(pDrlg->dwStartSeed == pOrgDrlg->dwStartSeed);
				CHECK(pDrlg->dwGameLowSeed == pOrgDrlg->dwGameLowSeed);
				CHECK(pDrlg->dwFlags == pOrgDrlg->dwFlags);
				for (int i = 0; i < 4; i++)
					CheckContent(&pDrlg->pRooms[i], &pOrgDrlg->pRooms[i], false);
				CheckContent(pDrlg->pRoomEx, pOrgDrlg->pRoomEx, false);
				CHECK(pDrlg->unk0x3DC == pOrgDrlg->unk0x3DC);
				CHECK(pDrlg->unk0x3DD[0] == pOrgDrlg->unk0x3DD[0]);
				CHECK(pDrlg->unk0x3DD[1] == pOrgDrlg->unk0x3DD[1]);
				CHECK(pDrlg->unk0x3DD[2] == pOrgDrlg->unk0x3DD[2]);
				CHECK(pDrlg->unk0x3E0[0] == pOrgDrlg->unk0x3E0[0]);
				CHECK(pDrlg->unk0x3E0[1] == pOrgDrlg->unk0x3E0[1]);
				// pGame
				CHECK(pDrlg->nDifficulty == pOrgDrlg->nDifficulty);
				// pad0x3ED
				CHECK(pDrlg->pfAutomap == nullptr);
				CHECK(pOrgDrlg->pfAutomap == nullptr);
				CHECK(pDrlg->pfTownAutomap == nullptr);
				CHECK(pOrgDrlg->pfTownAutomap == nullptr);
				CHECK(pDrlg->nStaffTombLevel == pOrgDrlg->nStaffTombLevel);
				CHECK(pDrlg->nBossTombLevel == pOrgDrlg->nBossTombLevel);
				// pTiles (loaded from d2cmp, no need to check?)
				CHECK(pDrlg->bJungleInterlink == pOrgDrlg->bJungleInterlink);
				CheckSameContent(pDrlg->pWarp,pOrgDrlg->pWarp);

				const D2DrlgLevelStrc* pLevel = pDrlg->pLevel;
				const D2DrlgLevelStrc* pOrgLevel = pOrgDrlg->pLevel;
				int levelIdx = 0;
				while (pLevel && pOrgLevel)
				{
					CAPTURE(levelIdx);
					CHECK(pLevel->pDrlg == pDrlg);
					CHECK(pOrgLevel->pDrlg == pOrgDrlg);

					CHECK(pLevel->nLevelId == pOrgLevel->nLevelId);
					CHECK(pLevel->nLevelType == pOrgLevel->nLevelType);
					CHECK(pLevel->nDrlgType == pOrgLevel->nDrlgType);
					CHECK(pLevel->dwFlags == pOrgLevel->dwFlags);
					CHECK(pLevel->pSeed.lSeed == pOrgLevel->pSeed.lSeed);
					CHECK(pLevel->dwInitSeed == pOrgLevel->dwInitSeed);
					CHECK(pLevel->pLevelCoords.nPosX == pOrgLevel->pLevelCoords.nPosX);
					CHECK(pLevel->pLevelCoords.nPosY == pOrgLevel->pLevelCoords.nPosY);
					CHECK(pLevel->pLevelCoords.nWidth == pOrgLevel->pLevelCoords.nWidth);
					CHECK(pLevel->pLevelCoords.nHeight == pOrgLevel->pLevelCoords.nHeight);

					CHECK(pLevel->nRooms == pOrgLevel->nRooms);
					D2RoomExStrc* pRoomEx = pLevel->pFirstRoomEx;
					D2RoomExStrc* pOrgRoomEx = pOrgLevel->pFirstRoomEx;
					while (pRoomEx && pOrgRoomEx)
					{
						CHECK(pRoomEx->pLevel == pLevel);
						CHECK(pOrgRoomEx->pLevel == pOrgLevel);
						CheckContent(pRoomEx, pOrgRoomEx, true);

						pRoomEx = pRoomEx->pRoomExNext;
						pOrgRoomEx = pOrgRoomEx->pRoomExNext;
					}
					CHECK(pRoomEx == nullptr);
					CHECK(pOrgRoomEx == nullptr);

					CHECK(pLevel->nCoordLists == pOrgLevel->nCoordLists);
					CHECK(pLevel->nTileInfo == pOrgLevel->nTileInfo);
					CHECK(pLevel->nRoomCoords == pOrgLevel->nRoomCoords);
					CHECK(pLevel->nJungleDefs == pOrgLevel->nJungleDefs);
					CHECK(pLevel->bActive == pOrgLevel->bActive);
					CHECK(pLevel->dwInactiveFrames == pOrgLevel->dwInactiveFrames);

					pLevel = pLevel->pNextLevel;
					pOrgLevel = pOrgLevel->pNextLevel;
					levelIdx++;
				}
				CHECK(pLevel == nullptr);
				CHECK(pOrgLevel == nullptr);
				DRLG_FreeDrlg(pDrlg);
				Original_DRLG_FreeDrlg(pOrgDrlg);
			}
		}
	}

}

TEST_CASE("ExpireStatListEx")
{
	D2StatListExStrc rootList{};
	D2StatListExStrc childList1{};
	D2StatListExStrc childList2{};
	childList1.pNextLink = &childList2;
	childList1.pPrevLink = nullptr;
	childList1.pParent = &rootList;
	childList2.pNextLink = nullptr;
	childList2.pPrevLink = &childList1;
	childList2.pParent = &rootList;
	rootList.pMyLastList = &childList2;
	rootList.pMyStats = &childList2;
	rootList.dwFlags |= STATLIST_EXTENDED;

	D2Common_10474(nullptr, &childList2);

	CHECK(rootList.pMyLastList == &childList1);
	CHECK(rootList.pMyStats == &childList1);

	D2Common_10474(nullptr, &childList1);
	CHECK(rootList.pMyLastList == nullptr);
	CHECK(rootList.pMyStats == nullptr);
}

// This was used to trace and debug, but not sure what to do with it anymore
TEST_CASE("STATLIST_InsertStatOrFail")
{
	D2StatListExStrc rootList{};
	rootList.dwOwnerId = 1;
	rootList.dwFlags = STATLIST_EXTENDED;

	// Base stats
	{
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x0000)->nValue = 0x001e;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x0001)->nValue = 0x000a;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x0002)->nValue = 0x0014;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x0003)->nValue = 0x0019;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x0007)->nValue = 0x3700;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x0009)->nValue = 0x0a00;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x000b)->nValue = 0x5c00;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x000c)->nValue = 0x0001;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x0043)->nValue = 0x0064;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x0044)->nValue = 0x0064;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.Stats, 0x0045)->nValue = 0x0064;
	}

	rootList.pMyLastList = nullptr; // TODO;
	rootList.pOwner = nullptr; // TODO;

	// Full stats
	{
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0000)->nValue = 0x001e;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0001)->nValue = 0x000a;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0002)->nValue = 0x0014;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0003)->nValue = 0x0019;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0007)->nValue = 0x3700;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0009)->nValue = 0x0a00;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x000b)->nValue = 0x5c00;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x000c)->nValue = 0x0001;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0015)->nValue = 0x0003;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0016)->nValue = 0x0006;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x001f)->nValue = 0x0005;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0043)->nValue = 0x0064;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0044)->nValue = 0x0064;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0045)->nValue = 0x0064;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0048)->nValue = 0x0028;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x0049)->nValue = 0x0028;
		STATLIST_InsertStatOrFail_6FDB6970(nullptr, &rootList.FullStats, 0x00ac)->nValue = 0x0002;
	}

	// Mod stats
	{
		STATLIST_InsertStatModOrFail_6FDB7690(&rootList, 0x00000000);
		STATLIST_InsertStatModOrFail_6FDB7690(&rootList, 0x00010000);
		STATLIST_InsertStatModOrFail_6FDB7690(&rootList, 0x00020000);
		STATLIST_InsertStatModOrFail_6FDB7690(&rootList, 0x00030000);
		STATLIST_InsertStatModOrFail_6FDB7690(&rootList, 0x00070000);
		STATLIST_InsertStatModOrFail_6FDB7690(&rootList, 0x00090000);
		STATLIST_InsertStatModOrFail_6FDB7690(&rootList, 0x000b0000);
		STATLIST_InsertStatModOrFail_6FDB7690(&rootList, 0x000c0000);
	}

	// const size_t SizeStatFlags = 2 * sizeof(uint32_t) * (sgptDataTables->nStatesTxtRecordCount + 31) / 32;
	const size_t SizeStatFlags = 0x35;
	rootList.StatFlags = (uint32_t*)D2_CALLOC_SERVER(nullptr, SizeStatFlags);
	rootList.StatFlags[0x03] = 0x00000200;
	rootList.StatFlags[0x09] = 0x00000200;
	rootList.StatFlags[0x14] = 0x0c1f2a00;
	rootList.StatFlags[0x15] = 0x0c1f2b00;
	rootList.StatFlags[0x21] = 0x00000001;
	rootList.StatFlags[0x24] = 0x00000008;
	rootList.StatFlags[0x25] = 0x00000008;
	rootList.StatFlags[0x26] = 0x00000068;
	rootList.StatFlags[0x27] = 0x00000068;
	rootList.StatFlags[0x29] = 0xffffffff;
	rootList.StatFlags[0x30] = 0x00010000;

	rootList.fpCallBack = nullptr; // Something in D2Client

	D2StatListExStrc statList{};
	statList.dwOwnerType = UNIT_ITEM;
	statList.dwOwnerId = 7;
	statList.dwFlags = STATLIST_EXTENDED;
	STATLIST_InsertStatOrFail_6FDB6970(nullptr, &statList.Stats, 0x0015)->nValue = 0x03;
	STATLIST_InsertStatOrFail_6FDB6970(nullptr, &statList.Stats, 0x0016)->nValue = 0x06;
	STATLIST_InsertStatOrFail_6FDB6970(nullptr, &statList.Stats, 0x0048)->nValue = 0x1c;
	STATLIST_InsertStatOrFail_6FDB6970(nullptr, &statList.Stats, 0x0049)->nValue = 0x1c;
	statList.pParent = &rootList;



	//Original_D2Common_10474();
	//
	//D2Common_10474();

	D2_FREE_SERVER(nullptr, rootList.StatFlags);
}
