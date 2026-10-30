#include <D2CommonTestDefines.h>

#ifdef DRLG_ROOMTILE_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2CMP.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgRoomTile.h>


TEST_SUITE("D2DrlgRoomTileTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD88860" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_GetTileCache, dll_base + 0x00048860);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			int nType{};
			uint32_t nPackedTileInformation{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, nType, nPackedTileInformation);
			const auto original_result = original(&original_pDrlgRoom, nType, nPackedTileInformation);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD889C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_InitWallTileData, dll_base + 0x000489C0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgTileDataStrc* moo_ppTileData{};
			D2TileLibraryEntryStrc moo_pTileLibraryEntry{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgTileDataStrc* original_ppTileData{};
			D2TileLibraryEntryStrc original_pTileLibraryEntry{};
			int nX{};
			int nY{};
			uint32_t nPackedTileInformation{};
			int nTileType{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgTileDataStrc*& ppTileData,
				D2TileLibraryEntryStrc& pTileLibraryEntry
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_ppTileData, moo_pTileLibraryEntry);
			setup_data(original_pDrlgRoom, original_ppTileData, original_pTileLibraryEntry);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, &moo_ppTileData, nX, nY, nPackedTileInformation, &moo_pTileLibraryEntry, nTileType);
			const auto original_result = original(&original_pDrlgRoom, &original_ppTileData, nX, nY, nPackedTileInformation, &original_pTileLibraryEntry, nTileType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_ppTileData, original_ppTileData, "Comparing ppTileData");
			SKIP_MOO_CHECK_EQ(moo_pTileLibraryEntry, original_pTileLibraryEntry, "Comparing pTileLibraryEntry");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD88AC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_InitializeTileDataFlags, dll_base + 0x00048AC0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgTileDataStrc moo_pTileData{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgTileDataStrc original_pTileData{};
			uint32_t nTileFlags{};
			int nType{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgTileDataStrc& pTileData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pTileData);
			setup_data(original_pDrlgRoom, original_pTileData);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pTileData, nTileFlags, nType, nX, nY);
			original(&original_pDrlgRoom, &original_pTileData, nTileFlags, nType, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pTileData, original_pTileData, "Comparing pTileData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD88BE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_AddTilePresetUnits, dll_base + 0x00048BE0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgTileDataStrc moo_pTileData{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgTileDataStrc original_pTileData{};
			uint32_t nPackedTileInformation{};
			int nX{};
			int nY{};
			int nTileType{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgTileDataStrc& pTileData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pTileData);
			setup_data(original_pDrlgRoom, original_pTileData);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pTileData, nPackedTileInformation, nX, nY, nTileType);
			original(&original_pDrlgRoom, &original_pTileData, nPackedTileInformation, nX, nY, nTileType);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pTileData, original_pTileData, "Comparing pTileData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD88DD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_InitTileData, dll_base + 0x00048DD0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgTileDataStrc moo_pTileData{};
			D2TileLibraryEntryStrc moo_pTileLibraryEntry{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgTileDataStrc original_pTileData{};
			D2TileLibraryEntryStrc original_pTileLibraryEntry{};
			int nX{};
			int nY{};
			uint32_t nPackedTileInformation{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgTileDataStrc& pTileData,
				D2TileLibraryEntryStrc& pTileLibraryEntry
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pTileData, moo_pTileLibraryEntry);
			setup_data(original_pDrlgRoom, original_pTileData, original_pTileLibraryEntry);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pTileData, nX, nY, nPackedTileInformation, &moo_pTileLibraryEntry);
			original(&original_pDrlgRoom, &original_pTileData, nX, nY, nPackedTileInformation, &original_pTileLibraryEntry);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pTileData, original_pTileData, "Comparing pTileData");
			SKIP_MOO_CHECK_EQ(moo_pTileLibraryEntry, original_pTileLibraryEntry, "Comparing pTileLibraryEntry");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD88E60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_InitFloorTileData, dll_base + 0x00048E60);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgTileDataStrc* moo_ppTileData{};
			D2TileLibraryEntryStrc moo_pTileLibraryEntry{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgTileDataStrc* original_ppTileData{};
			D2TileLibraryEntryStrc original_pTileLibraryEntry{};
			int nX{};
			int nY{};
			uint32_t nPackedTileInformation{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgTileDataStrc*& ppTileData,
				D2TileLibraryEntryStrc& pTileLibraryEntry
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_ppTileData, moo_pTileLibraryEntry);
			setup_data(original_pDrlgRoom, original_ppTileData, original_pTileLibraryEntry);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, &moo_ppTileData, nX, nY, nPackedTileInformation, &moo_pTileLibraryEntry);
			const auto original_result = original(&original_pDrlgRoom, &original_ppTileData, nX, nY, nPackedTileInformation, &original_pTileLibraryEntry);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_ppTileData, original_ppTileData, "Comparing ppTileData");
			SKIP_MOO_CHECK_EQ(moo_pTileLibraryEntry, original_pTileLibraryEntry, "Comparing pTileLibraryEntry");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD88F10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_InitShadowTileData, dll_base + 0x00048F10);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgTileDataStrc* moo_ppTileData{};
			D2TileLibraryEntryStrc moo_pTileLibraryEntry{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgTileDataStrc* original_ppTileData{};
			D2TileLibraryEntryStrc original_pTileLibraryEntry{};
			int nX{};
			int nY{};
			uint32_t nPackedTileInformation{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgTileDataStrc*& ppTileData,
				D2TileLibraryEntryStrc& pTileLibraryEntry
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_ppTileData, moo_pTileLibraryEntry);
			setup_data(original_pDrlgRoom, original_ppTileData, original_pTileLibraryEntry);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, &moo_ppTileData, nX, nY, nPackedTileInformation, &moo_pTileLibraryEntry);
			const auto original_result = original(&original_pDrlgRoom, &original_ppTileData, nX, nY, nPackedTileInformation, &original_pTileLibraryEntry);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_ppTileData, original_ppTileData, "Comparing ppTileData");
			SKIP_MOO_CHECK_EQ(moo_pTileLibraryEntry, original_pTileLibraryEntry, "Comparing pTileLibraryEntry");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD88FD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_InitTileShadow, dll_base + 0x00048FD0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			int nX{};
			int nY{};
			uint32_t nPackedTileInformation{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom, nX, nY, nPackedTileInformation);
			original(&original_pDrlgRoom, nX, nY, nPackedTileInformation);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89000" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_LoadInitRoomTiles, dll_base + 0x00049000);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgGridStrc moo_pTilePackedInfoGrid{};
			D2DrlgGridStrc moo_pTileTypeGrid{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgGridStrc original_pTilePackedInfoGrid{};
			D2DrlgGridStrc original_pTileTypeGrid{};
			BOOL bFillBlanks{};
			BOOL bKillEdgeX{};
			BOOL bKillEdgeY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgGridStrc& pTilePackedInfoGrid,
				D2DrlgGridStrc& pTileTypeGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pTilePackedInfoGrid, moo_pTileTypeGrid);
			setup_data(original_pDrlgRoom, original_pTilePackedInfoGrid, original_pTileTypeGrid);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pTilePackedInfoGrid, &moo_pTileTypeGrid, bFillBlanks, bKillEdgeX, bKillEdgeY);
			original(&original_pDrlgRoom, &original_pTilePackedInfoGrid, &original_pTileTypeGrid, bFillBlanks, bKillEdgeX, bKillEdgeY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pTilePackedInfoGrid, original_pTilePackedInfoGrid, "Comparing pTilePackedInfoGrid");
			SKIP_MOO_CHECK_EQ(moo_pTileTypeGrid, original_pTileTypeGrid, "Comparing pTileTypeGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89360" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_AddWarp, dll_base + 0x00049360);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			int nX{};
			int nY{};
			uint32_t nPackedTileInformation{};
			int nTileType{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, nX, nY, nPackedTileInformation, nTileType);
			const auto original_result = original(&original_pDrlgRoom, nX, nY, nPackedTileInformation, nTileType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89410" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_LoadWallWarpTiles, dll_base + 0x00049410);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgTileDataStrc moo_pTileData{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgTileDataStrc original_pTileData{};
			uint32_t nPackedTileInformation{};
			int nTileType{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgTileDataStrc& pTileData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pTileData);
			setup_data(original_pDrlgRoom, original_pTileData);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pTileData, nPackedTileInformation, nTileType);
			original(&original_pDrlgRoom, &original_pTileData, nPackedTileInformation, nTileType);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pTileData, original_pTileData, "Comparing pTileData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89590" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_LoadFloorWarpTiles, dll_base + 0x00049590);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			int nX{};
			int nY{};
			uint32_t nPackedTileInformation{};
			int nTileType{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom, nX, nY, nPackedTileInformation, nTileType);
			original(&original_pDrlgRoom, nX, nY, nPackedTileInformation, nTileType);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD897E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_GetLinkedTileData, dll_base + 0x000497E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc* moo_ppDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgRoomStrc* original_ppDrlgRoom{};
			BOOL bFloor{};
			uint32_t nPackedTileInformation{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgRoomStrc*& ppDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_ppDrlgRoom);
			setup_data(original_pDrlgRoom, original_ppDrlgRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, bFloor, nPackedTileInformation, nX, nY, &moo_ppDrlgRoom);
			const auto original_result = original(&original_pDrlgRoom, bFloor, nPackedTileInformation, nX, nY, &original_ppDrlgRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_ppDrlgRoom, original_ppDrlgRoom, "Comparing ppDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89930" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_AddLinkedTileData, dll_base + 0x00049930);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nTileType{};
			uint32_t nPackedTileInformation{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgRoom, nTileType, nPackedTileInformation, nX, nY);
			original(original_pMemPool, &original_pDrlgRoom, nTileType, nPackedTileInformation, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89AF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_LinkedTileDataManager, dll_base + 0x00049AF0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom1{};
			D2DrlgRoomStrc moo_pDrlgRoom2{};
			D2DrlgTileDataStrc moo_pTileData{};
			D2DrlgRoomStrc original_pDrlgRoom1{};
			D2DrlgRoomStrc original_pDrlgRoom2{};
			D2DrlgTileDataStrc original_pTileData{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nTileType{};
			uint32_t nPackedTileInformation{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom1,
				D2DrlgRoomStrc& pDrlgRoom2,
				D2DrlgTileDataStrc& pTileData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom1, moo_pDrlgRoom2, moo_pTileData);
			setup_data(original_pDrlgRoom1, original_pDrlgRoom2, original_pTileData);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgRoom1, &moo_pDrlgRoom2, &moo_pTileData, nTileType, nPackedTileInformation, nX, nY);
			original(original_pMemPool, &original_pDrlgRoom1, &original_pDrlgRoom2, &original_pTileData, nTileType, nPackedTileInformation, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom1, original_pDrlgRoom1, "Comparing pDrlgRoom1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom2, original_pDrlgRoom2, "Comparing pDrlgRoom2");
			SKIP_MOO_CHECK_EQ(moo_pTileData, original_pTileData, "Comparing pTileData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89CC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_GetCreateLinkedTileData, dll_base + 0x00049CC0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nTileType{};
			uint32_t nPackedTileInformation{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgRoom, nTileType, nPackedTileInformation, nX, nY);
			original(original_pMemPool, &original_pDrlgRoom, nTileType, nPackedTileInformation, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89E30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_CountAllTileTypes, dll_base + 0x00049E30);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgGridStrc moo_pTileInfoGrid{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgGridStrc original_pTileInfoGrid{};
			BOOL bCheckCoordinatesValidity{};
			BOOL bKillEdgeX{};
			BOOL bKillEdgeY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgGridStrc& pTileInfoGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pTileInfoGrid);
			setup_data(original_pDrlgRoom, original_pTileInfoGrid);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pTileInfoGrid, bCheckCoordinatesValidity, bKillEdgeX, bKillEdgeY);
			original(&original_pDrlgRoom, &original_pTileInfoGrid, bCheckCoordinatesValidity, bKillEdgeX, bKillEdgeY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pTileInfoGrid, original_pTileInfoGrid, "Comparing pTileInfoGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89F00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_CountWallWarpTiles, dll_base + 0x00049F00);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgGridStrc moo_pTileInfoGrid{};
			D2DrlgGridStrc moo_pTileTypeGrid{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgGridStrc original_pTileInfoGrid{};
			D2DrlgGridStrc original_pTileTypeGrid{};
			BOOL bKillEdgeX{};
			BOOL bKillEdgeY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgGridStrc& pTileInfoGrid,
				D2DrlgGridStrc& pTileTypeGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pTileInfoGrid, moo_pTileTypeGrid);
			setup_data(original_pDrlgRoom, original_pTileInfoGrid, original_pTileTypeGrid);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pTileInfoGrid, &moo_pTileTypeGrid, bKillEdgeX, bKillEdgeY);
			original(&original_pDrlgRoom, &original_pTileInfoGrid, &original_pTileTypeGrid, bKillEdgeX, bKillEdgeY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pTileInfoGrid, original_pTileInfoGrid, "Comparing pTileInfoGrid");
			SKIP_MOO_CHECK_EQ(moo_pTileTypeGrid, original_pTileTypeGrid, "Comparing pTileTypeGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89FA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_InitRoomGrids, dll_base + 0x00049FA0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom);
			original(&original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD89FD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_AddRoomMapTiles, dll_base + 0x00049FD0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom);
			original(&original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A010" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_AllocTileGrid, dll_base + 0x0004A010);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom);
			original(&original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A050" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_AllocTileData, dll_base + 0x0004A050);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom);
			original(&original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A130" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_ReallocRoofTileGrid, dll_base + 0x0004A130);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgTileGridStrc moo_pTileGrid{};
			D2DrlgTileGridStrc original_pTileGrid{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nAdditionalRoofs{};

			const auto setup_data = [](
				D2DrlgTileGridStrc& pTileGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTileGrid);
			setup_data(original_pTileGrid);

			// Call both implementations
			sut(moo_pMemPool, &moo_pTileGrid, nAdditionalRoofs);
			original(original_pMemPool, &original_pTileGrid, nAdditionalRoofs);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pTileGrid, original_pTileGrid, "Comparing pTileGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A1B0 (#10017)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_GetNumberOfShadowsFromRoom, dll_base + 0x0004A1B0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A1D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_FreeTileGrid, dll_base + 0x0004A1D0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom);
			original(&original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A2E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_FreeRoom, dll_base + 0x0004A2E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			BOOL bKeepRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom, bKeepRoom);
			original(&original_pDrlgRoom, bKeepRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A380" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOMTILE_LoadDT1FilesForRoom, dll_base + 0x0004A380);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom);
			original(&original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
}
#endif
