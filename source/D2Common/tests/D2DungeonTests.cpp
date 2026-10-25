#include <D2CommonTestDefines.h>

#ifdef DUNGEON_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Collision.h>
#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <GAME/Clients.h>
#include <GAME/Game.h>
#include <Units/Units.h>


TEST_SUITE("D2DungeonTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B8A0 (#10038)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_AllocAct, dll_base + 0x0004B8A0);
		
		SUBCASE("")
		{
			// Input data
			D2GameStrc moo_pGame{};
			D2GameStrc original_pGame{};
			uint8_t nActNo{};
			uint32_t nInitSeed{};
			BOOL bClient{};
			uint8_t nDifficulty{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nTownLevelId{};
			AUTOMAPFN pfAutoMap{};
			TOWNAUTOMAPFN pfTownAutoMap{};

			const auto setup_data = [](
				D2GameStrc& pGame
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pGame);
			setup_data(original_pGame);

			// Call both implementations
			const auto moo_result = sut(nActNo, nInitSeed, bClient, &moo_pGame, nDifficulty, moo_pMemPool, nTownLevelId, pfAutoMap, pfTownAutoMap);
			const auto original_result = original(nActNo, nInitSeed, bClient, &original_pGame, nDifficulty, original_pMemPool, nTownLevelId, pfAutoMap, pfTownAutoMap);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pGame, original_pGame, "Comparing pGame");
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B950 (#10039)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_FreeAct, dll_base + 0x0004B950);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			sut(&moo_pAct);
			original(&original_pAct);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B9D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetMemPoolFromAct, dll_base + 0x0004B9D0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B9E0 (#10026)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_ToggleRoomTilesEnableFlag, dll_base + 0x0004B9E0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			BOOL bEnabled{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, bEnabled);
			original(&original_pRoom, bEnabled);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BA20 (#10027)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetWarpTileFromRoomAndSourceLevelId, dll_base + 0x0004BA20);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2LvlWarpTxt* moo_ppLvlWarpTxtRecord{};
			D2ActiveRoomStrc original_pRoom{};
			D2LvlWarpTxt* original_ppLvlWarpTxtRecord{};
			int nSourceLevel{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2LvlWarpTxt*& ppLvlWarpTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_ppLvlWarpTxtRecord);
			setup_data(original_pRoom, original_ppLvlWarpTxtRecord);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nSourceLevel, &moo_ppLvlWarpTxtRecord);
			const auto original_result = original(&original_pRoom, nSourceLevel, &original_ppLvlWarpTxtRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_ppLvlWarpTxtRecord, original_ppLvlWarpTxtRecord, "Comparing ppLvlWarpTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BAB0 (#10028)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetLvlWarpTxtRecordFromRoomAndUnit, dll_base + 0x0004BAB0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2UnitStrc moo_pUnit{};
			D2ActiveRoomStrc original_pRoom{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pUnit);
			setup_data(original_pRoom, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pUnit);
			const auto original_result = original(&original_pRoom, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BAF0 (#10030)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetFloorTilesFromRoom, dll_base + 0x0004BAF0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			int moo_pFloorCount{};
			D2ActiveRoomStrc original_pRoom{};
			int original_pFloorCount{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				int& pFloorCount
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pFloorCount);
			setup_data(original_pRoom, original_pFloorCount);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pFloorCount);
			const auto original_result = original(&original_pRoom, &original_pFloorCount);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pFloorCount, original_pFloorCount, "Comparing pFloorCount");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BB20 (#10031)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetWallTilesFromRoom, dll_base + 0x0004BB20);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			int moo_pWallCount{};
			D2ActiveRoomStrc original_pRoom{};
			int original_pWallCount{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				int& pWallCount
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pWallCount);
			setup_data(original_pRoom, original_pWallCount);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pWallCount);
			const auto original_result = original(&original_pRoom, &original_pWallCount);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pWallCount, original_pWallCount, "Comparing pWallCount");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BB60 (#10032)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetRoofTilesFromRoom, dll_base + 0x0004BB60);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			int moo_pRoofCount{};
			D2ActiveRoomStrc original_pRoom{};
			int original_pRoofCount{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				int& pRoofCount
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pRoofCount);
			setup_data(original_pRoom, original_pRoofCount);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pRoofCount);
			const auto original_result = original(&original_pRoom, &original_pRoofCount);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pRoofCount, original_pRoofCount, "Comparing pRoofCount");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BBA0 (#10033)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetTileDataFromAct, dll_base + 0x0004BBA0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BBB0 (#10034)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetRoomCoordinates, dll_base + 0x0004BBB0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2DrlgCoordsStrc moo_pCoords{};
			D2ActiveRoomStrc original_pRoom{};
			D2DrlgCoordsStrc original_pCoords{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2DrlgCoordsStrc& pCoords
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pCoords);
			setup_data(original_pRoom, original_pCoords);

			// Call both implementations
			sut(&moo_pRoom, &moo_pCoords);
			original(&original_pRoom, &original_pCoords);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pCoords, original_pCoords, "Comparing pCoords");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BC10 (#10035)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetAdjacentRoomsListFromRoom, dll_base + 0x0004BC10);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc** moo_pppRoomList{};
			int moo_pNumRooms{};
			D2ActiveRoomStrc original_pRoom{};
			D2ActiveRoomStrc** original_pppRoomList{};
			int original_pNumRooms{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2ActiveRoomStrc**& pppRoomList,
				int& pNumRooms
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pppRoomList, moo_pNumRooms);
			setup_data(original_pRoom, original_pppRoomList, original_pNumRooms);

			// Call both implementations
			sut(&moo_pRoom, &moo_pppRoomList, &moo_pNumRooms);
			original(&original_pRoom, &original_pppRoomList, &original_pNumRooms);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pppRoomList, original_pppRoomList, "Comparing pppRoomList");
			SKIP_MOO_CHECK_EQ(moo_pNumRooms, original_pNumRooms, "Comparing pNumRooms");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BC50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_AllocRoom, dll_base + 0x0004BC50);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgCoordsStrc moo_pDrlgCoords{};
			D2DrlgRoomTilesStrc moo_pRoomTiles{};
			D2DrlgActStrc original_pAct{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgCoordsStrc original_pDrlgCoords{};
			D2DrlgRoomTilesStrc original_pRoomTiles{};
			int nLowSeed{};
			uint32_t dwFlags{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgCoordsStrc& pDrlgCoords,
				D2DrlgRoomTilesStrc& pRoomTiles
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct, moo_pDrlgRoom, moo_pDrlgCoords, moo_pRoomTiles);
			setup_data(original_pAct, original_pDrlgRoom, original_pDrlgCoords, original_pRoomTiles);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, &moo_pDrlgRoom, &moo_pDrlgCoords, &moo_pRoomTiles, nLowSeed, dwFlags);
			const auto original_result = original(&original_pAct, &original_pDrlgRoom, &original_pDrlgCoords, &original_pRoomTiles, nLowSeed, dwFlags);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoords, original_pDrlgCoords, "Comparing pDrlgCoords");
			SKIP_MOO_CHECK_EQ(moo_pRoomTiles, original_pRoomTiles, "Comparing pRoomTiles");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BD90 (#10040)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_DoRoomsTouchOrOverlap, dll_base + 0x0004BD90);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_ptFirst{};
			D2ActiveRoomStrc moo_ptSecond{};
			D2ActiveRoomStrc original_ptFirst{};
			D2ActiveRoomStrc original_ptSecond{};

			const auto setup_data = [](
				D2ActiveRoomStrc& ptFirst,
				D2ActiveRoomStrc& ptSecond
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_ptFirst, moo_ptSecond);
			setup_data(original_ptFirst, original_ptSecond);

			// Call both implementations
			const auto moo_result = sut(&moo_ptFirst, &moo_ptSecond);
			const auto original_result = original(&original_ptFirst, &original_ptSecond);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_ptFirst, original_ptFirst, "Comparing ptFirst");
			SKIP_MOO_CHECK_EQ(moo_ptSecond, original_ptSecond, "Comparing ptSecond");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BE30 (#10043)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_AreTileCoordinatesInsideRoom, dll_base + 0x0004BE30);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY);
			const auto original_result = original(&original_pRoom, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BE90 (#10048)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_CheckRoomsOverlapping_BROKEN, dll_base + 0x0004BE90);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pPrimary{};
			D2ActiveRoomStrc moo_pSecondary{};
			D2ActiveRoomStrc original_pPrimary{};
			D2ActiveRoomStrc original_pSecondary{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pPrimary,
				D2ActiveRoomStrc& pSecondary
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPrimary, moo_pSecondary);
			setup_data(original_pPrimary, original_pSecondary);

			// Call both implementations
			const auto moo_result = sut(&moo_pPrimary, &moo_pSecondary);
			const auto original_result = original(&original_pPrimary, &original_pSecondary);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPrimary, original_pPrimary, "Comparing pPrimary");
			SKIP_MOO_CHECK_EQ(moo_pSecondary, original_pSecondary, "Comparing pSecondary");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Commmon.0x6FD8BF00 (#10051)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_FindRoomByTileCoordinates, dll_base + 0x0004BF00);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, nX, nY);
			const auto original_result = original(&original_pAct, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BF50 (#10050)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetAdjacentRoomByTileCoordinates, dll_base + 0x0004BF50);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY);
			const auto original_result = original(&original_pRoom, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8BFF0 (#10049)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_CallRoomCallback, dll_base + 0x0004BFF0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			ROOMCALLBACKFN pfnRoomCallback{};
			void* moo_pArgs = nullptr;
			void* original_pArgs = nullptr;

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, pfnRoomCallback, moo_pArgs);
			original(&original_pRoom, pfnRoomCallback, original_pArgs);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pArgs, original_pArgs, "Comparing pArgs");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C080 (#10052)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10052, dll_base + 0x0004C080);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			RECT moo_pRect{};
			D2ActiveRoomStrc original_pRoom{};
			RECT original_pRect{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				RECT& pRect
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pRect);
			setup_data(original_pRoom, original_pRect);

			// Call both implementations
			sut(&moo_pRoom, &moo_pRect);
			original(&original_pRoom, &original_pRect);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pRect, original_pRect, "Comparing pRect");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C170 (#10053)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetSubtileRect, dll_base + 0x0004C170);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			RECT moo_pRect{};
			D2ActiveRoomStrc original_pRoom{};
			RECT original_pRect{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				RECT& pRect
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pRect);
			setup_data(original_pRoom, original_pRect);

			// Call both implementations
			sut(&moo_pRoom, &moo_pRect);
			original(&original_pRoom, &original_pRect);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pRect, original_pRect, "Comparing pRect");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C210 (#10054)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetRGB_IntensityFromRoom, dll_base + 0x0004C210);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			uint8_t moo_pIntensity{};
			uint8_t moo_pRed{};
			uint8_t moo_pGreen{};
			uint8_t moo_pBlue{};
			D2ActiveRoomStrc original_pRoom{};
			uint8_t original_pIntensity{};
			uint8_t original_pRed{};
			uint8_t original_pGreen{};
			uint8_t original_pBlue{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				uint8_t& pIntensity,
				uint8_t& pRed,
				uint8_t& pGreen,
				uint8_t& pBlue
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pIntensity, moo_pRed, moo_pGreen, moo_pBlue);
			setup_data(original_pRoom, original_pIntensity, original_pRed, original_pGreen, original_pBlue);

			// Call both implementations
			sut(&moo_pRoom, &moo_pIntensity, &moo_pRed, &moo_pGreen, &moo_pBlue);
			original(&original_pRoom, &original_pIntensity, &original_pRed, &original_pGreen, &original_pBlue);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pIntensity, original_pIntensity, "Comparing pIntensity");
			SKIP_MOO_CHECK_EQ(moo_pRed, original_pRed, "Comparing pRed");
			SKIP_MOO_CHECK_EQ(moo_pGreen, original_pGreen, "Comparing pGreen");
			SKIP_MOO_CHECK_EQ(moo_pBlue, original_pBlue, "Comparing pBlue");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C240 (#10041)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_FindRoomBySubtileCoordinates, dll_base + 0x0004C240);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, nX, nY);
			const auto original_result = original(&original_pAct, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C290" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_AreSubtileCoordinatesInsideRoom, dll_base + 0x0004C290);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordsStrc moo_pDrlgCoords{};
			D2DrlgCoordsStrc original_pDrlgCoords{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgCoordsStrc& pDrlgCoords
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoords);
			setup_data(original_pDrlgCoords);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgCoords, nX, nY);
			const auto original_result = original(&original_pDrlgCoords, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoords, original_pDrlgCoords, "Comparing pDrlgCoords");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C2F0 (#10046)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_FindActSpawnLocation, dll_base + 0x0004C2F0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			int moo_pX{};
			int moo_pY{};
			D2DrlgActStrc original_pAct{};
			int original_pX{};
			int original_pY{};
			int nLevelId{};
			int nTileIndex{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct, moo_pX, moo_pY);
			setup_data(original_pAct, original_pX, original_pY);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, nLevelId, nTileIndex, &moo_pX, &moo_pY);
			const auto original_result = original(&original_pAct, nLevelId, nTileIndex, &original_pX, &original_pY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C340 (#10045)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_FindActSpawnLocationEx, dll_base + 0x0004C340);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			int moo_pX{};
			int moo_pY{};
			D2DrlgActStrc original_pAct{};
			int original_pX{};
			int original_pY{};
			int nLevelId{};
			int nTileIndex{};
			int nUnitSize{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct, moo_pX, moo_pY);
			setup_data(original_pAct, original_pX, original_pY);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, nLevelId, nTileIndex, &moo_pX, &moo_pY, nUnitSize);
			const auto original_result = original(&original_pAct, nLevelId, nTileIndex, &original_pX, &original_pY, nUnitSize);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C4A0 (#10029)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetFirstUnitInRoom, dll_base + 0x0004C4A0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C4E0 (#10100)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_IncreaseAlliedCountOfRoom, dll_base + 0x0004C4E0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom);
			original(&original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Comon.0x6FD8C4F0 (#10036)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetAlliedCountFromRoom, dll_base + 0x0004C4F0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C510 (#10101)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_DecreaseAlliedCountOfRoom, dll_base + 0x0004C510);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom);
			original(&original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C550" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetUnitListFromRoom, dll_base + 0x0004C550);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C580" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetUnitUpdateListFromRoom, dll_base + 0x0004C580);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			BOOL bUpdate{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, bUpdate);
			const auto original_result = original(&original_pRoom, bUpdate);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C5C0 (#10055)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetPresetUnitsFromRoom, dll_base + 0x0004C5C0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C600" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetCollisionGridFromRoom, dll_base + 0x0004C600);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C630" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_SetCollisionGridInRoom, dll_base + 0x0004C630);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2RoomCollisionGridStrc moo_pCollisionGrid{};
			D2ActiveRoomStrc original_pRoom{};
			D2RoomCollisionGridStrc original_pCollisionGrid{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2RoomCollisionGridStrc& pCollisionGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pCollisionGrid);
			setup_data(original_pRoom, original_pCollisionGrid);

			// Call both implementations
			sut(&moo_pRoom, &moo_pCollisionGrid);
			original(&original_pRoom, &original_pCollisionGrid);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pCollisionGrid, original_pCollisionGrid, "Comparing pCollisionGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C660 (#10063)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_SetClientIsInSight, dll_base + 0x0004C660);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2ActiveRoomStrc moo_pRoom{};
			D2DrlgActStrc original_pAct{};
			D2ActiveRoomStrc original_pRoom{};
			int nLevelId{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct, moo_pRoom);
			setup_data(original_pAct, original_pRoom);

			// Call both implementations
			sut(&moo_pAct, nLevelId, nX, nY, &moo_pRoom);
			original(&original_pAct, nLevelId, nX, nY, &original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C6B0 (#10064)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_UnsetClientIsInSight, dll_base + 0x0004C6B0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2ActiveRoomStrc moo_pRoom{};
			D2DrlgActStrc original_pAct{};
			D2ActiveRoomStrc original_pRoom{};
			int nLevelId{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct, moo_pRoom);
			setup_data(original_pAct, original_pRoom);

			// Call both implementations
			sut(&moo_pAct, nLevelId, nX, nY, &moo_pRoom);
			original(&original_pAct, nLevelId, nX, nY, &original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C700 (#10062)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_ChangeClientRoom, dll_base + 0x0004C700);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom1{};
			D2ActiveRoomStrc moo_pRoom2{};
			D2ActiveRoomStrc original_pRoom1{};
			D2ActiveRoomStrc original_pRoom2{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom1,
				D2ActiveRoomStrc& pRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom1, moo_pRoom2);
			setup_data(original_pRoom1, original_pRoom2);

			// Call both implementations
			sut(&moo_pRoom1, &moo_pRoom2);
			original(&original_pRoom1, &original_pRoom2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom1, original_pRoom1, "Comparing pRoom1");
			SKIP_MOO_CHECK_EQ(moo_pRoom2, original_pRoom2, "Comparing pRoom2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C730 (#10065)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_StreamRoomAtCoords, dll_base + 0x0004C730);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, nX, nY);
			const auto original_result = original(&original_pAct, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C770 (#10056)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetRoomFromAct, dll_base + 0x0004C770);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C7A0 (#10057)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetLevelIdFromRoom, dll_base + 0x0004C7A0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C7C0 (#10058)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetWarpDestinationLevel, dll_base + 0x0004C7C0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nSourceLevel{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nSourceLevel);
			const auto original_result = original(&original_pRoom, nSourceLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C7E0 (#10059)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetLevelIdFromPopulatedRoom, dll_base + 0x0004C7E0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C800 (#10060)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_HasWaypoint, dll_base + 0x0004C800);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C840 (#10061)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetPickedLevelPrestFilePathFromRoom, dll_base + 0x0004C840);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C860 (#10066)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_AllocDrlgDelete, dll_base + 0x0004C860);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nUnitType{};
			D2UnitGUID nUnitGuid{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nUnitType, nUnitGuid);
			original(&original_pRoom, nUnitType, nUnitGuid);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C8B0 (#10067)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_FreeDrlgDelete, dll_base + 0x0004C8B0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom);
			original(&original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C910 (#10068)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetDrlgDeleteFromRoom, dll_base + 0x0004C910);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C940 (#10069)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetARoomInClientSight, dll_base + 0x0004C940);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C980 (#10070)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetARoomInSightButWithoutClient, dll_base + 0x0004C980);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2ActiveRoomStrc moo_pRoom{};
			D2DrlgActStrc original_pAct{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct, moo_pRoom);
			setup_data(original_pAct, original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, &moo_pRoom);
			const auto original_result = original(&original_pAct, &original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8C9E0 (#10071)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_TestRoomCanUnTile, dll_base + 0x0004C9E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2ActiveRoomStrc moo_pRoom{};
			D2DrlgActStrc original_pAct{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct, moo_pRoom);
			setup_data(original_pAct, original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, &moo_pRoom);
			const auto original_result = original(&original_pAct, &original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CA60 (#10072)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetRoomStatusFlags, dll_base + 0x0004CA60);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CA80 (#10073)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10073, dll_base + 0x0004CA80);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CAE0 (#10074)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10074, dll_base + 0x0004CAE0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CB10 (#10075)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10075, dll_base + 0x0004CB10);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			BOOL bSet{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, bSet);
			original(&original_pRoom, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CB60 (#10079)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_AddClientToRoom, dll_base + 0x0004CB60);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ClientStrc moo_pClient{};
			D2ActiveRoomStrc original_pRoom{};
			D2ClientStrc original_pClient{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2ClientStrc& pClient
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pClient);
			setup_data(original_pRoom, original_pClient);

			// Call both implementations
			sut(&moo_pRoom, &moo_pClient);
			original(&original_pRoom, &original_pClient);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pClient, original_pClient, "Comparing pClient");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CC50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_UpdateClientListOfRoom, dll_base + 0x0004CC50);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom);
			original(&original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CD10 (#10080)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_RemoveClientFromRoom, dll_base + 0x0004CD10);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ClientStrc moo_pClient{};
			D2ActiveRoomStrc original_pRoom{};
			D2ClientStrc original_pClient{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2ClientStrc& pClient
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pClient);
			setup_data(original_pRoom, original_pClient);

			// Call both implementations
			sut(&moo_pRoom, &moo_pClient);
			original(&original_pRoom, &original_pClient);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pClient, original_pClient, "Comparing pClient");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CDF0 (#10081)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10081_GetTileCountFromRoom, dll_base + 0x0004CDF0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CE40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_FreeRoom, dll_base + 0x0004CE40);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(moo_pMemPool, &moo_pRoom);
			original(original_pMemPool, &original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8CF10 (#10076)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_RemoveRoomFromAct, dll_base + 0x0004CF10);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2ActiveRoomStrc moo_pRoom{};
			D2DrlgActStrc original_pAct{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct, moo_pRoom);
			setup_data(original_pAct, original_pRoom);

			// Call both implementations
			sut(&moo_pAct, &moo_pRoom);
			original(&original_pAct, &original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D000 (#10077)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10077, dll_base + 0x0004D000);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom1{};
			D2ActiveRoomStrc moo_pRoom2{};
			D2ActiveRoomStrc original_pRoom1{};
			D2ActiveRoomStrc original_pRoom2{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom1,
				D2ActiveRoomStrc& pRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom1, moo_pRoom2);
			setup_data(original_pRoom1, original_pRoom2);

			// Call both implementations
			sut(&moo_pRoom1, &moo_pRoom2);
			original(&original_pRoom1, &original_pRoom2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom1, original_pRoom1, "Comparing pRoom1");
			SKIP_MOO_CHECK_EQ(moo_pRoom2, original_pRoom2, "Comparing pRoom2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D030 (#10078)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_UpdateAndFreeInactiveRooms, dll_base + 0x0004D030);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			sut(&moo_pAct);
			original(&original_pAct);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D040 (#10044)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_CheckLOSDraw, dll_base + 0x0004D040);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D060" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetEnvironmentFromAct, dll_base + 0x0004D060);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D090 (#10088)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetDrlgFromAct, dll_base + 0x0004D090);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD912D0 (#10089)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetInitSeedFromAct, dll_base + 0x000512D0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D0C0 (#10007)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetRoomExFromRoom, dll_base + 0x0004D0C0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D0D0 (#10086)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_IsTownLevelId, dll_base + 0x0004D0D0);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D0E0 (#10082)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_IsRoomInTown, dll_base + 0x0004D0E0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D100 (#10083)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10083_Return0, dll_base + 0x0004D100);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D130 (#10084)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10084, dll_base + 0x0004D130);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D140 (#10085)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetTownLevelIdFromActNo, dll_base + 0x0004D140);
		
		SUBCASE("")
		{
			uint8_t nAct{};

			// Call both implementations
			const auto moo_result = sut(nAct);
			const auto original_result = original(nAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D180 (#10087)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10087, dll_base + 0x0004D180);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D1C0 (#10090)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetNumberOfPopulatedRoomsInLevel, dll_base + 0x0004D1C0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};
			int nLevelId{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, nLevelId);
			const auto original_result = original(&original_pAct, nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D1E0 (#10025)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetWarpCoordinatesFromRoom, dll_base + 0x0004D1E0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D220 (#10091)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_UpdateWarpRoomSelect, dll_base + 0x0004D220);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nLevelId{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nLevelId);
			original(&original_pRoom, nLevelId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D260 (#10092)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_UpdateWarpRoomDeselect, dll_base + 0x0004D260);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nLevelId{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nLevelId);
			original(&original_pRoom, nLevelId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D2A0 (#10093)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_UpdatePops, dll_base + 0x0004D2A0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			BOOL bOtherRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, bOtherRoom);
			original(&original_pRoom, nX, nY, bOtherRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D2E0 (#10094)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetTombStoneTileCoords, dll_base + 0x0004D2E0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2CoordStrc* moo_ppTombStoneTiles{};
			int moo_pnTombStoneTiles{};
			D2ActiveRoomStrc original_pRoom{};
			D2CoordStrc* original_ppTombStoneTiles{};
			int original_pnTombStoneTiles{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2CoordStrc*& ppTombStoneTiles,
				int& pnTombStoneTiles
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_ppTombStoneTiles, moo_pnTombStoneTiles);
			setup_data(original_pRoom, original_ppTombStoneTiles, original_pnTombStoneTiles);

			// Call both implementations
			sut(&moo_pRoom, &moo_ppTombStoneTiles, &moo_pnTombStoneTiles);
			original(&original_pRoom, &original_ppTombStoneTiles, &original_pnTombStoneTiles);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_ppTombStoneTiles, original_ppTombStoneTiles, "Comparing ppTombStoneTiles");
			SKIP_MOO_CHECK_EQ(moo_pnTombStoneTiles, original_pnTombStoneTiles, "Comparing pnTombStoneTiles");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D300 (#10095)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10095, dll_base + 0x0004D300);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY);
			const auto original_result = original(&original_pRoom, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D3A0 (#10096)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10096, dll_base + 0x0004D3A0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY);
			const auto original_result = original(&original_pRoom, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D3C0 (#10097)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetRoomCoordList, dll_base + 0x0004D3C0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D3D0 (#10098)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetPortalLevelArrayFromPortalFlags, dll_base + 0x0004D3D0);
		
		SUBCASE("")
		{
			// Input data
			int* moo_ppLevels{};
			int moo_pnLevels{};
			int* original_ppLevels{};
			int original_pnLevels{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nFlags{};

			const auto setup_data = [](
				int*& ppLevels,
				int& pnLevels
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_ppLevels, moo_pnLevels);
			setup_data(original_ppLevels, original_pnLevels);

			// Call both implementations
			sut(moo_pMemPool, nFlags, &moo_ppLevels, &moo_pnLevels);
			original(original_pMemPool, nFlags, &original_ppLevels, &original_pnLevels);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_ppLevels, original_ppLevels, "Comparing ppLevels");
			SKIP_MOO_CHECK_EQ(moo_pnLevels, original_pnLevels, "Comparing pnLevels");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D4B0 (#10099)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetPortalFlagFromLevelId, dll_base + 0x0004D4B0);
		
		SUBCASE("")
		{
			int nPortalLevelId{};

			// Call both implementations
			const auto moo_result = sut(nPortalLevelId);
			const auto original_result = original(nPortalLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D4F0 (#10037)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetTownLevelIdFromAct, dll_base + 0x0004D4F0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D520 (#10047)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GetHoradricStaffTombLevelId, dll_base + 0x0004D520);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D540 (#10102)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_ToggleHasPortalFlag, dll_base + 0x0004D540);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			BOOL bReset{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, bReset);
			original(&original_pRoom, bReset);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D560 (#10104)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_AnimateTiles, dll_base + 0x0004D560);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom);
			original(&original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D580 (#10105)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_InitRoomTileAnimation, dll_base + 0x0004D580);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2ActiveRoomStrc moo_pRoom1{};
			D2ActiveRoomStrc moo_pRoom2{};
			D2DrlgActStrc original_pAct{};
			D2ActiveRoomStrc original_pRoom1{};
			D2ActiveRoomStrc original_pRoom2{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				D2ActiveRoomStrc& pRoom1,
				D2ActiveRoomStrc& pRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct, moo_pRoom1, moo_pRoom2);
			setup_data(original_pAct, original_pRoom1, original_pRoom2);

			// Call both implementations
			sut(&moo_pAct, &moo_pRoom1, &moo_pRoom2);
			original(&original_pAct, &original_pRoom1, &original_pRoom2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			SKIP_MOO_CHECK_EQ(moo_pRoom1, original_pRoom1, "Comparing pRoom1");
			SKIP_MOO_CHECK_EQ(moo_pRoom2, original_pRoom2, "Comparing pRoom2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D5C0 (#10103)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_SetActCallbackFunc, dll_base + 0x0004D5C0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgActStrc original_pAct{};
			ACTCALLBACKFN pActCallbackFunction{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAct);
			setup_data(original_pAct);

			// Call both implementations
			sut(&moo_pAct, pActCallbackFunction);
			original(&original_pAct, pActCallbackFunction);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D600 (#10106)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_SaveKilledUnitGUID, dll_base + 0x0004D600);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			D2UnitGUID nUnitGUID{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nUnitGUID);
			original(&original_pRoom, nUnitGUID);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D690 (#10107)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_ClientToGameTileCoords, dll_base + 0x0004D690);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(&moo_pX, &moo_pY);
			original(&original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D870 (#10108)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_ClientToGameSubtileCoords, dll_base + 0x0004D870);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(&moo_pX, &moo_pY);
			original(&original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D8A0 (#10109)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_ClientToGameCoords, dll_base + 0x0004D8A0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(&moo_pX, &moo_pY);
			original(&original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D6E0 (#10110)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GameTileToClientCoords, dll_base + 0x0004D6E0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(&moo_pX, &moo_pY);
			original(&original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D630 (#10111)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GameSubtileToClientCoords, dll_base + 0x0004D630);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(&moo_pX, &moo_pY);
			original(&original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D660 (#10112)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GameToClientCoords, dll_base + 0x0004D660);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(&moo_pX, &moo_pY);
			original(&original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D8C0 (#10113)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GameTileToSubtileCoords, dll_base + 0x0004D8C0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(&moo_pX, &moo_pY);
			original(&original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D710 (#10114)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_ClientTileDrawPositionToGameCoords, dll_base + 0x0004D710);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(nX, nY, &moo_pX, &moo_pY);
			original(nX, nY, &original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D790 (#10115)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GameToClientTileDrawPositionCoords, dll_base + 0x0004D790);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(nX, nY, &moo_pX, &moo_pY);
			original(nX, nY, &original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D7D0 (#10116)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_ClientSubileDrawPositionToGameCoords, dll_base + 0x0004D7D0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(nX, nY, &moo_pX, &moo_pY);
			original(nX, nY, &original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D830 (#10117)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DUNGEON_GameToClientSubtileDrawPositionCoords, dll_base + 0x0004D830);
		
		SUBCASE("")
		{
			// Input data
			int moo_pX{};
			int moo_pY{};
			int original_pX{};
			int original_pY{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pX, moo_pY);
			setup_data(original_pX, original_pY);

			// Call both implementations
			sut(nX, nY, &moo_pX, &moo_pY);
			original(nX, nY, &original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
}
#endif
