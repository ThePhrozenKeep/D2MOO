#include <D2CommonTestDefines.h>

#ifdef COLLISION_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2CMP.h>
#include <D2Collision.h>
#include <Drlg/D2DrlgDrlg.h>


TEST_SUITE("D2CollisionTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD41000" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_COLLISION_FirstFn_6FD41000, dll_base + 0x00001000);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2DrlgTileDataStrc moo_pTileData{};
			D2TileLibraryEntryStrc moo_pTileLibraryEntry{};
			D2ActiveRoomStrc original_pRoom{};
			D2DrlgTileDataStrc original_pTileData{};
			D2TileLibraryEntryStrc original_pTileLibraryEntry{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2DrlgTileDataStrc& pTileData,
				D2TileLibraryEntryStrc& pTileLibraryEntry
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pTileData, moo_pTileLibraryEntry);
			setup_data(original_pRoom, original_pTileData, original_pTileLibraryEntry);

			// Call both implementations
			sut(&moo_pRoom, &moo_pTileData, &moo_pTileLibraryEntry);
			original(&original_pRoom, &original_pTileData, &original_pTileLibraryEntry);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pTileData, original_pTileData, "Comparing pTileData");
			SKIP_MOO_CHECK_EQ(moo_pTileLibraryEntry, original_pTileLibraryEntry, "Comparing pTileLibraryEntry");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD411F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD411F0, dll_base + 0x000011F0);
		
		SUBCASE("")
		{
			// Input data
			D2RoomCollisionGridStrc moo_pCollisionGrid{};
			D2TileLibraryEntryStrc moo_pTileLibraryEntry{};
			D2RoomCollisionGridStrc original_pCollisionGrid{};
			D2TileLibraryEntryStrc original_pTileLibraryEntry{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2RoomCollisionGridStrc& pCollisionGrid,
				D2TileLibraryEntryStrc& pTileLibraryEntry
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCollisionGrid, moo_pTileLibraryEntry);
			setup_data(original_pCollisionGrid, original_pTileLibraryEntry);

			// Call both implementations
			sut(&moo_pCollisionGrid, &moo_pTileLibraryEntry, nX, nY);
			original(&original_pCollisionGrid, &original_pTileLibraryEntry, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCollisionGrid, original_pCollisionGrid, "Comparing pCollisionGrid");
			SKIP_MOO_CHECK_EQ(moo_pTileLibraryEntry, original_pTileLibraryEntry, "Comparing pTileLibraryEntry");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD412B0 (#10018)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10018_Return0, dll_base + 0x000012B0);
		
		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD412C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_AllocRoomCollisionGrid, dll_base + 0x000012C0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD413E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD413E0, dll_base + 0x000013E0);
		
		SUBCASE("")
		{
			// Input data
			D2RoomCollisionGridStrc moo_pCollisionGrid{};
			D2RoomCollisionGridStrc moo_pAdjacentCollisionGrid{};
			D2DrlgTileDataStrc moo_pTiles{};
			D2RoomCollisionGridStrc original_pCollisionGrid{};
			D2RoomCollisionGridStrc original_pAdjacentCollisionGrid{};
			D2DrlgTileDataStrc original_pTiles{};
			int nTiles{};
			BOOL bRemoveOldFlags{};

			const auto setup_data = [](
				D2RoomCollisionGridStrc& pCollisionGrid,
				D2RoomCollisionGridStrc& pAdjacentCollisionGrid,
				D2DrlgTileDataStrc& pTiles
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCollisionGrid, moo_pAdjacentCollisionGrid, moo_pTiles);
			setup_data(original_pCollisionGrid, original_pAdjacentCollisionGrid, original_pTiles);

			// Call both implementations
			sut(&moo_pCollisionGrid, &moo_pAdjacentCollisionGrid, &moo_pTiles, nTiles, bRemoveOldFlags);
			original(&original_pCollisionGrid, &original_pAdjacentCollisionGrid, &original_pTiles, nTiles, bRemoveOldFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCollisionGrid, original_pCollisionGrid, "Comparing pCollisionGrid");
			SKIP_MOO_CHECK_EQ(moo_pAdjacentCollisionGrid, original_pAdjacentCollisionGrid, "Comparing pAdjacentCollisionGrid");
			SKIP_MOO_CHECK_EQ(moo_pTiles, original_pTiles, "Comparing pTiles");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD41610" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_FreeRoomCollisionGrid, dll_base + 0x00001610);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD41650 (#10118)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckMask, dll_base + 0x00001650);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY, nMask);
			const auto original_result = original(&original_pRoom, nX, nY, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD41720 (#10127)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetMask, dll_base + 0x00001720);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nMask);
			original(&original_pRoom, nX, nY, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD417F0 (#10123)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetMask, dll_base + 0x000017F0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nMask);
			original(&original_pRoom, nX, nY, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD418C0 (#10120)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckMaskWithSizeXY, dll_base + 0x000018C0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			unsigned int nSizeX{};
			unsigned int nSizeY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY, nSizeX, nSizeY, nMask);
			const auto original_result = original(&original_pRoom, nX, nY, nSizeX, nSizeY, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD41B40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckCollisionMaskForBoundingBox, dll_base + 0x00001B40);
		
		SUBCASE("")
		{
			// Input data
			D2RoomCollisionGridStrc moo_pCollisionGrid{};
			D2BoundingBoxStrc moo_pBoundingBox{};
			D2RoomCollisionGridStrc original_pCollisionGrid{};
			D2BoundingBoxStrc original_pBoundingBox{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2RoomCollisionGridStrc& pCollisionGrid,
				D2BoundingBoxStrc& pBoundingBox
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCollisionGrid, moo_pBoundingBox);
			setup_data(original_pCollisionGrid, original_pBoundingBox);

			// Call both implementations
			const auto moo_result = sut(&moo_pCollisionGrid, &moo_pBoundingBox, nMask);
			const auto original_result = original(&original_pCollisionGrid, &original_pBoundingBox, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCollisionGrid, original_pCollisionGrid, "Comparing pCollisionGrid");
			SKIP_MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD41BE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_AdaptBoundingBoxToGrid, dll_base + 0x00001BE0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2BoundingBoxStrc moo_pBoundingBox{};
			D2BoundingBoxStrc moo_pBoundingBoxes{};
			D2ActiveRoomStrc original_pRoom{};
			D2BoundingBoxStrc original_pBoundingBox{};
			D2BoundingBoxStrc original_pBoundingBoxes{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2BoundingBoxStrc& pBoundingBox,
				D2BoundingBoxStrc& pBoundingBoxes
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pBoundingBox, moo_pBoundingBoxes);
			setup_data(original_pRoom, original_pBoundingBox, original_pBoundingBoxes);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pBoundingBox, &moo_pBoundingBoxes);
			const auto original_result = original(&original_pRoom, &original_pBoundingBox, &original_pBoundingBoxes);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
			SKIP_MOO_CHECK_EQ(moo_pBoundingBoxes, original_pBoundingBoxes, "Comparing pBoundingBoxes");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD41CA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckCollisionMaskForBoundingBoxRecursively, dll_base + 0x00001CA0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2BoundingBoxStrc moo_pBoundingBox{};
			D2ActiveRoomStrc original_pRoom{};
			D2BoundingBoxStrc original_pBoundingBox{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2BoundingBoxStrc& pBoundingBox
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pBoundingBox);
			setup_data(original_pRoom, original_pBoundingBox);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pBoundingBox, nMask);
			const auto original_result = original(&original_pRoom, &original_pBoundingBox, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD41DE0 (#10121)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckMaskWithPattern, dll_base + 0x00001DE0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			int nCollisionPattern{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY, nCollisionPattern, nMask);
			const auto original_result = original(&original_pRoom, nX, nY, nCollisionPattern, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD42000" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckCollisionMaskWithAdjacentCells, dll_base + 0x00002000);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY, nMask);
			const auto original_result = original(&original_pRoom, nX, nY, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD42670" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckCollisionMask, dll_base + 0x00002670);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY, nMask);
			const auto original_result = original(&original_pRoom, nX, nY, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD42740 (#10122)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckAnyCollisionWithPattern, dll_base + 0x00002740);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			int nCollisionPattern{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY, nCollisionPattern, nMask);
			const auto original_result = original(&original_pRoom, nX, nY, nCollisionPattern, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD42A30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckAnyCollisionWithAdjacentCells, dll_base + 0x00002A30);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY, nMask);
			const auto original_result = original(&original_pRoom, nX, nY, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD43080 (#10119)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckMaskWithSize, dll_base + 0x00003080);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			int nUnitSize{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY, nUnitSize, nMask);
			const auto original_result = original(&original_pRoom, nX, nY, nUnitSize, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD432A0 (#10128)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetMaskWithSize, dll_base + 0x000032A0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			int nUnitSize{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nUnitSize, nMask);
			original(&original_pRoom, nX, nY, nUnitSize, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD434B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetCollisionMask, dll_base + 0x000034B0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nMask);
			original(&original_pRoom, nX, nY, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD43580" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetCollisionMaskForBoundingBoxRecursively, dll_base + 0x00003580);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2BoundingBoxStrc moo_pBoundingBox{};
			D2ActiveRoomStrc original_pRoom{};
			D2BoundingBoxStrc original_pBoundingBox{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2BoundingBoxStrc& pBoundingBox
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pBoundingBox);
			setup_data(original_pRoom, original_pBoundingBox);

			// Call both implementations
			sut(&moo_pRoom, &moo_pBoundingBox, nMask);
			original(&original_pRoom, &original_pBoundingBox, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD436F0 (#10130)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetMaskWithPattern, dll_base + 0x000036F0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			int nCollisionPattern{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nCollisionPattern, nMask);
			original(&original_pRoom, nX, nY, nCollisionPattern, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD439D0 (#10124)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetMaskWithSize, dll_base + 0x000039D0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			int nUnitSize{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nUnitSize, nMask);
			original(&original_pRoom, nX, nY, nUnitSize, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD43C10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetCollisionMask, dll_base + 0x00003C10);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nMask);
			original(&original_pRoom, nX, nY, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD43CE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetCollisionMaskForBoundingBoxRecursively, dll_base + 0x00003CE0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2BoundingBoxStrc moo_pBoundingBox{};
			D2ActiveRoomStrc original_pRoom{};
			D2BoundingBoxStrc original_pBoundingBox{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2BoundingBoxStrc& pBoundingBox
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pBoundingBox);
			setup_data(original_pRoom, original_pBoundingBox);

			// Call both implementations
			sut(&moo_pRoom, &moo_pBoundingBox, nMask);
			original(&original_pRoom, &original_pBoundingBox, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD43E60 (#10126)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetMaskWithPattern, dll_base + 0x00003E60);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			int nCollisionPattern{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nCollisionPattern, nMask);
			original(&original_pRoom, nX, nY, nCollisionPattern, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44140 (#10125)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetMaskWithSizeXY, dll_base + 0x00004140);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			unsigned int nSizeX{};
			unsigned int nSizeY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nSizeX, nSizeY, nMask);
			original(&original_pRoom, nX, nY, nSizeX, nSizeY, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44370" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetCollisionMaskForBoundingBox, dll_base + 0x00004370);
		
		SUBCASE("")
		{
			// Input data
			D2RoomCollisionGridStrc moo_pCollisionGrid{};
			D2BoundingBoxStrc moo_pBoundingBox{};
			D2RoomCollisionGridStrc original_pCollisionGrid{};
			D2BoundingBoxStrc original_pBoundingBox{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2RoomCollisionGridStrc& pCollisionGrid,
				D2BoundingBoxStrc& pBoundingBox
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCollisionGrid, moo_pBoundingBox);
			setup_data(original_pCollisionGrid, original_pBoundingBox);

			// Call both implementations
			sut(&moo_pCollisionGrid, &moo_pBoundingBox, nMask);
			original(&original_pCollisionGrid, &original_pBoundingBox, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCollisionGrid, original_pCollisionGrid, "Comparing pCollisionGrid");
			SKIP_MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD443E0 (#10129)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetMaskWithSizeXY, dll_base + 0x000043E0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			unsigned int nSizeX{};
			unsigned int nSizeY{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom, nX, nY, nSizeX, nSizeY, nMask);
			original(&original_pRoom, nX, nY, nSizeX, nSizeY, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44600" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetCollisionMaskForBoundingBox, dll_base + 0x00004600);
		
		SUBCASE("")
		{
			// Input data
			D2RoomCollisionGridStrc moo_pCollisionGrid{};
			D2BoundingBoxStrc moo_pBoundingBox{};
			D2RoomCollisionGridStrc original_pCollisionGrid{};
			D2BoundingBoxStrc original_pBoundingBox{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2RoomCollisionGridStrc& pCollisionGrid,
				D2BoundingBoxStrc& pBoundingBox
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCollisionGrid, moo_pBoundingBox);
			setup_data(original_pCollisionGrid, original_pBoundingBox);

			// Call both implementations
			sut(&moo_pCollisionGrid, &moo_pBoundingBox, nMask);
			original(&original_pCollisionGrid, &original_pBoundingBox, nMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCollisionGrid, original_pCollisionGrid, "Comparing pCollisionGrid");
			SKIP_MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44660 (#10131)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_TryMoveUnitCollisionMask, dll_base + 0x00004660);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};
			int nUnitSize{};
			uint16_t nCollisionMask{};
			uint16_t nMoveConditionMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX1, nY1, nX2, nY2, nUnitSize, nCollisionMask, nMoveConditionMask);
			const auto original_result = original(&original_pRoom, nX1, nY1, nX2, nY2, nUnitSize, nCollisionMask, nMoveConditionMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44910" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CreateBoundingBox, dll_base + 0x00004910);
		
		SUBCASE("")
		{
			// Input data
			D2BoundingBoxStrc moo_pBoundingBox{};
			D2BoundingBoxStrc original_pBoundingBox{};
			int nCenterX{};
			int nCenterY{};
			unsigned int nSizeX{};
			unsigned int nSizeY{};

			const auto setup_data = [](
				D2BoundingBoxStrc& pBoundingBox
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pBoundingBox);
			setup_data(original_pBoundingBox);

			// Call both implementations
			sut(&moo_pBoundingBox, nCenterX, nCenterY, nSizeX, nSizeY);
			original(&original_pBoundingBox, nCenterX, nCenterY, nSizeX, nSizeY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44950 (#10132)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_TryTeleportUnitCollisionMask, dll_base + 0x00004950);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};
			int nCollisionPattern{};
			uint16_t nFootprintCollisionMask{};
			uint16_t nMoveConditionMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX1, nY1, nX2, nY2, nCollisionPattern, nFootprintCollisionMask, nMoveConditionMask);
			const auto original_result = original(&original_pRoom, nX1, nY1, nX2, nY2, nCollisionPattern, nFootprintCollisionMask, nMoveConditionMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44BB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ForceTeleportUnitCollisionMaskAndGetCollision, dll_base + 0x00004BB0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom1{};
			D2ActiveRoomStrc moo_pRoom2{};
			D2ActiveRoomStrc original_pRoom1{};
			D2ActiveRoomStrc original_pRoom2{};
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};
			int nUnitSize{};
			uint16_t nFootprintCollisionMask{};
			uint16_t nMoveConditionMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom1,
				D2ActiveRoomStrc& pRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom1, moo_pRoom2);
			setup_data(original_pRoom1, original_pRoom2);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom1, nX1, nY1, &moo_pRoom2, nX2, nY2, nUnitSize, nFootprintCollisionMask, nMoveConditionMask);
			const auto original_result = original(&original_pRoom1, nX1, nY1, &original_pRoom2, nX2, nY2, nUnitSize, nFootprintCollisionMask, nMoveConditionMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom1, original_pRoom1, "Comparing pRoom1");
			SKIP_MOO_CHECK_EQ(moo_pRoom2, original_pRoom2, "Comparing pRoom2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44E00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_TeleportUnitCollisionMask, dll_base + 0x00004E00);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom1{};
			D2ActiveRoomStrc moo_pRoom2{};
			D2ActiveRoomStrc original_pRoom1{};
			D2ActiveRoomStrc original_pRoom2{};
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};
			int nUnitSize{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom1,
				D2ActiveRoomStrc& pRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom1, moo_pRoom2);
			setup_data(original_pRoom1, original_pRoom2);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom1, nX1, nY1, &moo_pRoom2, nX2, nY2, nUnitSize, nMask);
			const auto original_result = original(&original_pRoom1, nX1, nY1, &original_pRoom2, nX2, nY2, nUnitSize, nMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom1, original_pRoom1, "Comparing pRoom1");
			SKIP_MOO_CHECK_EQ(moo_pRoom2, original_pRoom2, "Comparing pRoom2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44FF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_TrySetUnitCollisionMask, dll_base + 0x00004FF0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom1{};
			D2ActiveRoomStrc moo_pRoom2{};
			D2ActiveRoomStrc original_pRoom1{};
			D2ActiveRoomStrc original_pRoom2{};
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};
			int nCollisionPattern{};
			uint16_t nFootprintCollisionMask{};
			uint16_t nMoveConditionMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom1,
				D2ActiveRoomStrc& pRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom1, moo_pRoom2);
			setup_data(original_pRoom1, original_pRoom2);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom1, nX1, nY1, &moo_pRoom2, nX2, nY2, nCollisionPattern, nFootprintCollisionMask, nMoveConditionMask);
			const auto original_result = original(&original_pRoom1, nX1, nY1, &original_pRoom2, nX2, nY2, nCollisionPattern, nFootprintCollisionMask, nMoveConditionMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom1, original_pRoom1, "Comparing pRoom1");
			SKIP_MOO_CHECK_EQ(moo_pRoom2, original_pRoom2, "Comparing pRoom2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD451D0 (#10133)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetUnitCollisionMask, dll_base + 0x000051D0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom1{};
			D2ActiveRoomStrc moo_pRoom2{};
			D2ActiveRoomStrc original_pRoom1{};
			D2ActiveRoomStrc original_pRoom2{};
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};
			int nCollisionPattern{};
			uint16_t nCollisionMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom1,
				D2ActiveRoomStrc& pRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom1, moo_pRoom2);
			setup_data(original_pRoom1, original_pRoom2);

			// Call both implementations
			sut(&moo_pRoom1, nX1, nY1, &moo_pRoom2, nX2, nY2, nCollisionPattern, nCollisionMask);
			original(&original_pRoom1, nX1, nY1, &original_pRoom2, nX2, nY2, nCollisionPattern, nCollisionMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom1, original_pRoom1, "Comparing pRoom1");
			SKIP_MOO_CHECK_EQ(moo_pRoom2, original_pRoom2, "Comparing pRoom2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD459D0 (#10135)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinatesWithMaxDistance, dll_base + 0x000059D0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2CoordStrc moo_pSpawnPoint{};
			D2ActiveRoomStrc original_pRoom{};
			D2CoordStrc original_pSpawnPoint{};
			int nUnitSize{};
			unsigned int nMask{};
			BOOL bAllowNeighborRooms{};
			int nMaxDistance{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2CoordStrc& pSpawnPoint
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pSpawnPoint);
			setup_data(original_pRoom, original_pSpawnPoint);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pSpawnPoint, nUnitSize, nMask, bAllowNeighborRooms, nMaxDistance);
			const auto original_result = original(&original_pRoom, &original_pSpawnPoint, nUnitSize, nMask, bAllowNeighborRooms, nMaxDistance);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD45A00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinatesImpl, dll_base + 0x00005A00);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2CoordStrc moo_ptSpawnPoint{};
			D2CoordStrc moo_pFieldCoord{};
			D2ActiveRoomStrc original_pRoom{};
			D2CoordStrc original_ptSpawnPoint{};
			D2CoordStrc original_pFieldCoord{};
			int nUnitSize{};
			unsigned int nMask{};
			unsigned int nFieldMask{};
			BOOL bAllowNeighborRooms{};
			int nMaxDistance{};
			int nPosIncrementValue{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2CoordStrc& ptSpawnPoint,
				D2CoordStrc& pFieldCoord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_ptSpawnPoint, moo_pFieldCoord);
			setup_data(original_pRoom, original_ptSpawnPoint, original_pFieldCoord);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_ptSpawnPoint, &moo_pFieldCoord, nUnitSize, nMask, nFieldMask, bAllowNeighborRooms, nMaxDistance, nPosIncrementValue);
			const auto original_result = original(&original_pRoom, &original_ptSpawnPoint, &original_pFieldCoord, nUnitSize, nMask, nFieldMask, bAllowNeighborRooms, nMaxDistance, nPosIncrementValue);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_ptSpawnPoint, original_ptSpawnPoint, "Comparing ptSpawnPoint");
			SKIP_MOO_CHECK_EQ(moo_pFieldCoord, original_pFieldCoord, "Comparing pFieldCoord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD46280 (#10134)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinates, dll_base + 0x00006280);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2CoordStrc moo_pSpawnPoint{};
			D2ActiveRoomStrc original_pRoom{};
			D2CoordStrc original_pSpawnPoint{};
			int nUnitSize{};
			unsigned int nMask{};
			BOOL bAllowNeighborRooms{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2CoordStrc& pSpawnPoint
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pSpawnPoint);
			setup_data(original_pRoom, original_pSpawnPoint);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pSpawnPoint, nUnitSize, nMask, bAllowNeighborRooms);
			const auto original_result = original(&original_pRoom, &original_pSpawnPoint, nUnitSize, nMask, bAllowNeighborRooms);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD462B0 (#10137)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinatesEx, dll_base + 0x000062B0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2CoordStrc moo_pSpawnPoint{};
			D2ActiveRoomStrc original_pRoom{};
			D2CoordStrc original_pSpawnPoint{};
			int nUnitSize{};
			unsigned int nMask{};
			int nPosIncrementValue{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2CoordStrc& pSpawnPoint
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pSpawnPoint);
			setup_data(original_pRoom, original_pSpawnPoint);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pSpawnPoint, nUnitSize, nMask, nPosIncrementValue);
			const auto original_result = original(&original_pRoom, &original_pSpawnPoint, nUnitSize, nMask, nPosIncrementValue);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD462E0 (#10138)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinatesWithField, dll_base + 0x000062E0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2CoordStrc moo_pSpawnPoint{};
			D2CoordStrc moo_pFieldCoord{};
			D2ActiveRoomStrc original_pRoom{};
			D2CoordStrc original_pSpawnPoint{};
			D2CoordStrc original_pFieldCoord{};
			int nUnitSize{};
			unsigned int nMask{};
			unsigned int nFieldMask{};
			BOOL bAllowNeighborRooms{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2CoordStrc& pSpawnPoint,
				D2CoordStrc& pFieldCoord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pSpawnPoint, moo_pFieldCoord);
			setup_data(original_pRoom, original_pSpawnPoint, original_pFieldCoord);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pSpawnPoint, &moo_pFieldCoord, nUnitSize, nMask, nFieldMask, bAllowNeighborRooms);
			const auto original_result = original(&original_pRoom, &original_pSpawnPoint, &original_pFieldCoord, nUnitSize, nMask, nFieldMask, bAllowNeighborRooms);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
			SKIP_MOO_CHECK_EQ(moo_pFieldCoord, original_pFieldCoord, "Comparing pFieldCoord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD46310 (#10136)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10136, dll_base + 0x00006310);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2CoordStrc moo_pCoord{};
			D2ActiveRoomStrc* moo_ppRoom{};
			D2ActiveRoomStrc original_pRoom{};
			D2CoordStrc original_pCoord{};
			D2ActiveRoomStrc* original_ppRoom{};
			int a3{};
			uint16_t nMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2CoordStrc& pCoord,
				D2ActiveRoomStrc*& ppRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pCoord, moo_ppRoom);
			setup_data(original_pRoom, original_pCoord, original_ppRoom);

			// Call both implementations
			sut(&moo_pRoom, &moo_pCoord, a3, nMask, &moo_ppRoom);
			original(&original_pRoom, &original_pCoord, a3, nMask, &original_ppRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pCoord, original_pCoord, "Comparing pCoord");
			SKIP_MOO_CHECK_EQ(moo_ppRoom, original_ppRoom, "Comparing ppRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD46620" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetRoomBySubTileCoordinates, dll_base + 0x00006620);
		
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
}
#endif
