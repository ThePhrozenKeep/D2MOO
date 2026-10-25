#include <D2CommonTestDefines.h>

#ifdef DRLG_ANIM_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlgAnim.h>


TEST_SUITE("D2DrlgDrlgAnimTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75480" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGANIM_InitCache, dll_base + 0x00035480);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgTileDataStrc moo_pTileData{};
			D2DrlgStrc original_pDrlg{};
			D2DrlgTileDataStrc original_pTileData{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg,
				D2DrlgTileDataStrc& pTileData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg, moo_pTileData);
			setup_data(original_pDrlg, original_pTileData);

			// Call both implementations
			sut(&moo_pDrlg, &moo_pTileData);
			original(&original_pDrlg, &original_pTileData);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
			SKIP_MOO_CHECK_EQ(moo_pTileData, original_pTileData, "Comparing pTileData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75560" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGANIM_TestLoadAnimatedRoomTiles, dll_base + 0x00035560);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc moo_pTileTypeGrid{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgGridStrc original_pDrlgGrid{};
			D2DrlgGridStrc original_pTileTypeGrid{};
			int nTileType{};
			int nTileX{};
			int nTileY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgGridStrc& pDrlgGrid,
				D2DrlgGridStrc& pTileTypeGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pDrlgGrid, moo_pTileTypeGrid);
			setup_data(original_pDrlgRoom, original_pDrlgGrid, original_pTileTypeGrid);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pDrlgGrid, &moo_pTileTypeGrid, nTileType, nTileX, nTileY);
			original(&original_pDrlgRoom, &original_pDrlgGrid, &original_pTileTypeGrid, nTileType, nTileX, nTileY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
			SKIP_MOO_CHECK_EQ(moo_pTileTypeGrid, original_pTileTypeGrid, "Comparing pTileTypeGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD756B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGANIM_AnimateTiles, dll_base + 0x000356B0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75740" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGANIM_AllocAnimationTileGrids, dll_base + 0x00035740);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgGridStrc moo_pWallGrid{};
			D2DrlgGridStrc moo_pFloorGrid{};
			D2DrlgGridStrc moo_pShadowGrid{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgGridStrc original_pWallGrid{};
			D2DrlgGridStrc original_pFloorGrid{};
			D2DrlgGridStrc original_pShadowGrid{};
			int nAnimationSpeed{};
			int nWalls{};
			int nFloors{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgGridStrc& pWallGrid,
				D2DrlgGridStrc& pFloorGrid,
				D2DrlgGridStrc& pShadowGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pWallGrid, moo_pFloorGrid, moo_pShadowGrid);
			setup_data(original_pDrlgRoom, original_pWallGrid, original_pFloorGrid, original_pShadowGrid);

			// Call both implementations
			sut(&moo_pDrlgRoom, nAnimationSpeed, &moo_pWallGrid, nWalls, &moo_pFloorGrid, nFloors, &moo_pShadowGrid);
			original(&original_pDrlgRoom, nAnimationSpeed, &original_pWallGrid, nWalls, &original_pFloorGrid, nFloors, &original_pShadowGrid);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pWallGrid, original_pWallGrid, "Comparing pWallGrid");
			SKIP_MOO_CHECK_EQ(moo_pFloorGrid, original_pFloorGrid, "Comparing pFloorGrid");
			SKIP_MOO_CHECK_EQ(moo_pShadowGrid, original_pShadowGrid, "Comparing pShadowGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD757B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGANIM_AllocAnimationTileGrid, dll_base + 0x000357B0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgTileDataStrc moo_pTiles{};
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2DrlgTileDataStrc original_pTiles{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int nAnimationSpeed{};
			int nTiles{};
			int nUnused{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2DrlgTileDataStrc& pTiles,
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pTiles, moo_pDrlgGrid);
			setup_data(original_pDrlgRoom, original_pTiles, original_pDrlgGrid);

			// Call both implementations
			sut(&moo_pDrlgRoom, nAnimationSpeed, &moo_pTiles, nTiles, &moo_pDrlgGrid, nUnused);
			original(&original_pDrlgRoom, nAnimationSpeed, &original_pTiles, nTiles, &original_pDrlgGrid, nUnused);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pTiles, original_pTiles, "Comparing pTiles");
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75B00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGANIM_UpdateFrameInAdjacentRooms, dll_base + 0x00035B00);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom1{};
			D2DrlgRoomStrc moo_pDrlgRoom2{};
			D2DrlgRoomStrc original_pDrlgRoom1{};
			D2DrlgRoomStrc original_pDrlgRoom2{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom1,
				D2DrlgRoomStrc& pDrlgRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom1, moo_pDrlgRoom2);
			setup_data(original_pDrlgRoom1, original_pDrlgRoom2);

			// Call both implementations
			sut(&moo_pDrlgRoom1, &moo_pDrlgRoom2);
			original(&original_pDrlgRoom1, &original_pDrlgRoom2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom1, original_pDrlgRoom1, "Comparing pDrlgRoom1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom2, original_pDrlgRoom2, "Comparing pDrlgRoom2");
		}
	}
}
#endif
