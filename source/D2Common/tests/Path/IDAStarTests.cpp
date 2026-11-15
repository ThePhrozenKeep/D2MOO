#include <D2CommonTestDefines.h>

#ifdef IDASTAR_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Path/IDAStar.h>

#include <D2Collision.h>
#include <Drlg/D2DrlgDrlg.h>

#include <vector>


namespace
{
	// A single open 40x40 room at (80, 80), listing itself as its only adjacent room
	struct IDAStarRoom
	{
		static constexpr int origin = 80;
		static constexpr int size = 40;

		D2ActiveRoomStrc tRoom{};
		D2RoomCollisionGridStrc tCollisionGrid{};
		std::vector<uint16_t> collision_masks;
		D2ActiveRoomStrc* pRoomList[1]{};
		D2DynamicPathStrc tDynamicPath{};

		IDAStarRoom(const std::vector<D2PathPointStrc>& walls)
		{
			tRoom.tCoords.nSubtileX = origin;
			tRoom.tCoords.nSubtileY = origin;
			tRoom.tCoords.nSubtileWidth = size;
			tRoom.tCoords.nSubtileHeight = size;
			tRoom.tCoords.nTileXPos = origin / 5;
			tRoom.tCoords.nTileYPos = origin / 5;
			tRoom.tCoords.nTileWidth = size / 5;
			tRoom.tCoords.nTileHeight = size / 5;

			collision_masks.assign(size * size, COLLIDE_NONE);
			for (const auto& tWall : walls)
			{
				collision_masks[(tWall.Y - origin) * size + (tWall.X - origin)] = COLLIDE_WALL;
			}
			tCollisionGrid.pRoomCoords = tRoom.tCoords;
			tCollisionGrid.pCollisionMask = collision_masks.data();
			tRoom.pCollisionGrid = &tCollisionGrid;

			pRoomList[0] = &tRoom;
			tRoom.ppRoomList = pRoomList;
			tRoom.nNumRooms = 1;
		}

		IDAStarRoom(const IDAStarRoom&) = delete;
		IDAStarRoom& operator=(const IDAStarRoom&) = delete;
	};
}


TEST_SUITE("IDAStarTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA7970")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_IdaStar_6FDA7970, dll_base + 0x00067970);

		// The walls make the search reach nodes below the start and revisit nodes through a cheaper or costlier path
		const auto walls = GENERATE(
			std::vector<D2PathPointStrc>{},
			std::vector<D2PathPointStrc>{ { 102, 101 } },
			std::vector<D2PathPointStrc>{ { 102, 101 }, { 102, 102 } },
			std::vector<D2PathPointStrc>{ { 101, 101 }, { 102, 101 }, { 103, 101 } }
		);
		const auto target = GENERATE(D2PathPointStrc{ 105, 104 }, D2PathPointStrc{ 100, 110 }, D2PathPointStrc{ 95, 103 });

		SUBCASE("")
		{
			INFO("walls: ", walls.size(), ", target: ", target.X, ", ", target.Y);

			// Input data
			IDAStarRoom moo_room(walls);
			IDAStarRoom original_room(walls);
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [target](
				D2PathInfoStrc& pPathInfo,
				IDAStarRoom& room
			) {
				pPathInfo.tStartCoord = { 100, 100 };
				pPathInfo.tTargetCoord = target;
				pPathInfo.pStartRoom = &room.tRoom;
				pPathInfo.pTargetRoom = &room.tRoom;
				pPathInfo.nMinimumFScoreToEvaluate = 100;
				pPathInfo.nPathType = PATHTYPE_IDASTAR;
				pPathInfo.nCollisionPattern = COLLISION_PATTERN_NONE;
				pPathInfo.nCollisionMask = COLLIDE_WALL;
				pPathInfo.pDynamicPath = &room.tDynamicPath;
			};

			setup_data(moo_pPathInfo, moo_room);
			setup_data(original_pPathInfo, original_room);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo);
			const auto original_result = original(&original_pPathInfo);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
			MOO_CHECK_EQ(moo_room.tDynamicPath, original_room.tDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA7D40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_IDAStar_VisitNodes, dll_base + 0x00067D40);
		
		SUBCASE("")
		{
			// Input data
			D2PathIDAStarContextStrc moo_pContext{};
			D2PathInfoStrc moo_pPathInfo{};
			D2PathIDAStarContextStrc original_pContext{};
			D2PathInfoStrc original_pPathInfo{};
			int nFScoreCutoff{};

			const auto setup_data = [](
				D2PathIDAStarContextStrc& pContext,
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pContext, moo_pPathInfo);
			setup_data(original_pContext, original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pContext, nFScoreCutoff, &moo_pPathInfo);
			const auto original_result = original(&original_pContext, nFScoreCutoff, &original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pContext, original_pContext, "Comparing pContext");
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
}
#endif
