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

#include <Fixtures/CollisionRoomsFixture.h>


DYNAMIC_ARRAY_TYPE(uint16_t)


using CollisionFixture = CollisionRoomsFixture<NoOpFixture>;
using CollisionWithFieldFixture = CollisionFieldFixture<CollisionRoomsFixture<NoOpFixture>>;


namespace
{
	constexpr uint16_t collision_masks[] = {
		COLLIDE_ALL_MASK,
		COLLIDE_WALL,
		COLLIDE_MASK_INVALID,
		COLLIDE_MASK_PLAYER_PATH,
		COLLIDE_MASK_MONSTER_PATH,
		COLLIDE_MASK_SPAWN,
		COLLIDE_MASK_PLACEMENT,
		COLLIDE_MONSTER | COLLIDE_PLAYER,
		COLLIDE_NONE,
	};

	// Either one of the masks commonly used by the game or a random one
	uint16_t random_collision_mask()
	{
		const auto nIndex = random_unsigned_integer(0, std::size(collision_masks));
		if (nIndex < std::size(collision_masks))
		{
			return collision_masks[nIndex];
		}

		return static_cast<uint16_t>(random_unsigned_integer(0, 0xFFFF));
	}

	int random_signed_integer(int nMin, int nMax)
	{
		return nMin + static_cast<int>(random_unsigned_integer(0, nMax - nMin));
	}

	// D2CMP.#10085 returns the 5x5 subtile collision flags stored at offset 0x28 of the tile
	void randomize_tile_flags(D2TileLibraryEntryStrc& moo_pTile, D2TileLibraryEntryStrc& original_pTile)
	{
		auto moo_flags = reinterpret_cast<uint8_t*>(&moo_pTile) + 0x28;
		auto original_flags = reinterpret_cast<uint8_t*>(&original_pTile) + 0x28;
		for (int i = 0; i < 25; ++i)
		{
			const uint8_t nFlags = random_unsigned_integer(0, 2) ? static_cast<uint8_t>(random_unsigned_integer(0, 0xFF)) : 0;
			moo_flags[i] = nFlags;
			original_flags[i] = nFlags;
		}
	}

	void check_collision_maps(CollisionFixture& fixture)
	{
		for (int i = 0; i < CollisionFixture::room_count; ++i)
		{
			MOO_CHECK_EQ(fixture.moo_map.rooms[i], fixture.original_map.rooms[i], "Comparing rooms");

			auto wrapped_moo_masks = DynamicArray<uint16_t>{ fixture.moo_map.collision_masks[i].data(), static_cast<int>(fixture.moo_map.collision_masks[i].size()) };
			auto wrapped_original_masks = DynamicArray<uint16_t>{ fixture.original_map.collision_masks[i].data(), static_cast<int>(fixture.original_map.collision_masks[i].size()) };
			MOO_CHECK_EQ(wrapped_moo_masks, wrapped_original_masks, "Comparing collision masks");
		}
	}

	void setup_random_tiles(D2DrlgTileDataStrc* moo_pTiles, D2DrlgTileDataStrc* original_pTiles, int nTiles, const D2DrlgCoordsStrc& tCoords, D2TileLibraryEntryStrc* moo_pTileLibraryEntries, D2TileLibraryEntryStrc* original_pTileLibraryEntries, int nTileLibraryEntries)
	{
		for (int i = 0; i < nTiles; ++i)
		{
			// Tiles may be placed one tile outside of their room
			const int nPosX = random_signed_integer(-1, tCoords.nTileWidth);
			const int nPosY = random_signed_integer(-1, tCoords.nTileHeight);
			const uint32_t dwFlags = random_unsigned_integer(0, 0xFF);
			const int nEntry = random_unsigned_integer(0, nTileLibraryEntries - 1);

			const auto setup_data = [nPosX, nPosY, dwFlags](
				D2DrlgTileDataStrc& pTileData,
				D2TileLibraryEntryStrc& pTileLibraryEntry
			) {
				pTileData.nPosX = nPosX;
				pTileData.nPosY = nPosY;
				pTileData.nWidth = 1;
				pTileData.nHeight = 1;
				pTileData.dwFlags = dwFlags;
				pTileData.pTile = &pTileLibraryEntry;
			};

			setup_data(moo_pTiles[i], moo_pTileLibraryEntries[nEntry]);
			setup_data(original_pTiles[i], original_pTileLibraryEntries[nEntry]);
		}
	}
}


TEST_SUITE("D2CollisionTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD41000")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ReplaceTileCollision, dll_base + 0x00001000);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto has_new_tile = GENERATE(0, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(50);

			D2TileLibraryEntryStrc moo_pOldTile{};
			D2TileLibraryEntryStrc moo_pNewTile{};
			D2TileLibraryEntryStrc original_pOldTile{};
			D2TileLibraryEntryStrc original_pNewTile{};
			randomize_tile_flags(moo_pOldTile, original_pOldTile);
			randomize_tile_flags(moo_pNewTile, original_pNewTile);

			const int nRoomTileX = start_room == no_room ? origin_x / 5 : moo_map.rooms[start_room].tCoords.nTileXPos;
			const int nRoomTileY = start_room == no_room ? origin_y / 5 : moo_map.rooms[start_room].tCoords.nTileYPos;

			for (int nTileY = origin_y / 5 - 1; nTileY <= (origin_y + map_height) / 5; ++nTileY)
			{
				for (int nTileX = origin_x / 5 - 1; nTileX <= (origin_x + map_width) / 5; ++nTileX)
				{
					// Input data
					D2DrlgTileDataStrc moo_pTileData{};
					D2DrlgTileDataStrc original_pTileData{};

					const auto setup_data = [nTileX, nTileY, nRoomTileX, nRoomTileY](
						D2DrlgTileDataStrc& pTileData,
						D2TileLibraryEntryStrc& pOldTile
					) {
						pTileData.nPosX = nTileX - nRoomTileX;
						pTileData.nPosY = nTileY - nRoomTileY;
						pTileData.nWidth = 1;
						pTileData.nHeight = 1;
						pTileData.pTile = &pOldTile;
					};

					setup_data(moo_pTileData, moo_pOldTile);
					setup_data(original_pTileData, original_pOldTile);

					// Call both implementations
					sut(moo_room(start_room), &moo_pTileData, has_new_tile ? &moo_pNewTile : nullptr);
					original(original_room(start_room), &original_pTileData, has_new_tile ? &original_pNewTile : nullptr);

					// Compare potentially modified input data
					check_collision_maps(*this);
					MOO_CHECK_EQ(moo_pTileData, original_pTileData, "Comparing pTileData");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD411F0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD411F0, dll_base + 0x000011F0);

		REPEAT_5();

		const auto grid_index = GENERATE(0, center_room, 8);

		SUBCASE("")
		{
			reroll_masks(50);

			D2TileLibraryEntryStrc moo_pTileLibraryEntry{};
			D2TileLibraryEntryStrc original_pTileLibraryEntry{};
			randomize_tile_flags(moo_pTileLibraryEntry, original_pTileLibraryEntry);

			const auto& tCoords = moo_map.collision_grids[grid_index].pRoomCoords;

			// Tiles can stick out of the grid on the left and the bottom
			for (int nY = -4; nY <= tCoords.nSubtileHeight - 5; ++nY)
			{
				for (int nX = -4; nX <= tCoords.nSubtileWidth - 5; ++nX)
				{
					// Call both implementations
					sut(&moo_map.collision_grids[grid_index], &moo_pTileLibraryEntry, nX, nY);
					original(&original_map.collision_grids[grid_index], &original_pTileLibraryEntry, nX, nY);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}

		SUBCASE("no collision mask")
		{
			reroll_masks(50);

			moo_map.collision_grids[grid_index].pCollisionMask = nullptr;
			original_map.collision_grids[grid_index].pCollisionMask = nullptr;

			D2TileLibraryEntryStrc moo_pTileLibraryEntry{};
			D2TileLibraryEntryStrc original_pTileLibraryEntry{};
			randomize_tile_flags(moo_pTileLibraryEntry, original_pTileLibraryEntry);

			// Call both implementations
			sut(&moo_map.collision_grids[grid_index], &moo_pTileLibraryEntry, 0, 0);
			original(&original_map.collision_grids[grid_index], &original_pTileLibraryEntry, 0, 0);

			// Compare potentially modified input data
			check_collision_maps(*this);
		}

		SUBCASE("no collision grid")
		{
			D2TileLibraryEntryStrc moo_pTileLibraryEntry{};
			D2TileLibraryEntryStrc original_pTileLibraryEntry{};
			randomize_tile_flags(moo_pTileLibraryEntry, original_pTileLibraryEntry);

			// Call both implementations
			sut(nullptr, &moo_pTileLibraryEntry, 0, 0);
			original(nullptr, &original_pTileLibraryEntry, 0, 0);
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD412B0 (#10018)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10018_Return0, dll_base + 0x000012B0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			CHECK_EQ(moo_result, 0);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD412C0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_AllocRoomCollisionGrid, dll_base + 0x000012C0);
		const auto [sut_free, original_free] = make_function_pair(COLLISION_FreeRoomCollisionGrid, dll_base + 0x00001610);

		REPEAT_10();

		const auto room_index = GENERATE(center_room, 0, 2, 7);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			constexpr int tile_kinds = 3;
			constexpr int tiles_per_kind = 6;

			build_maps(removed_room);
			reroll_masks(50);

			D2TileLibraryEntryStrc moo_pTileLibraryEntries[4]{};
			D2TileLibraryEntryStrc original_pTileLibraryEntries[4]{};
			for (int i = 0; i < std::size(moo_pTileLibraryEntries); ++i)
			{
				randomize_tile_flags(moo_pTileLibraryEntries[i], original_pTileLibraryEntries[i]);
			}

			D2DrlgRoomTilesStrc moo_pRoomTiles[room_count]{};
			D2DrlgRoomTilesStrc original_pRoomTiles[room_count]{};
			D2DrlgTileDataStrc moo_pTiles[room_count][tile_kinds][tiles_per_kind]{};
			D2DrlgTileDataStrc original_pTiles[room_count][tile_kinds][tiles_per_kind]{};

			for (int i = 0; i < room_count; ++i)
			{
				for (int nKind = 0; nKind < tile_kinds; ++nKind)
				{
					setup_random_tiles(moo_pTiles[i][nKind], original_pTiles[i][nKind], tiles_per_kind, moo_map.rooms[i].tCoords, moo_pTileLibraryEntries, original_pTileLibraryEntries, std::size(moo_pTileLibraryEntries));
				}

				const int nWalls = random_unsigned_integer(0, tiles_per_kind);
				const int nFloors = random_unsigned_integer(0, tiles_per_kind);
				const int nRoofs = random_unsigned_integer(0, tiles_per_kind);

				const auto setup_data = [nWalls, nFloors, nRoofs](
					D2ActiveRoomStrc& pRoom,
					D2DrlgRoomTilesStrc& pRoomTiles,
					D2DrlgTileDataStrc (&pTiles)[tile_kinds][tiles_per_kind]
				) {
					pRoomTiles.pWallTiles = pTiles[0];
					pRoomTiles.nWalls = nWalls;
					pRoomTiles.pFloorTiles = pTiles[1];
					pRoomTiles.nFloors = nFloors;
					pRoomTiles.pRoofTiles = pTiles[2];
					pRoomTiles.nRoofs = nRoofs;
					pRoom.pRoomTiles = &pRoomTiles;
				};

				setup_data(moo_map.rooms[i], moo_pRoomTiles[i], moo_pTiles[i]);
				setup_data(original_map.rooms[i], original_pRoomTiles[i], original_pTiles[i]);
			}

			moo_map.rooms[room_index].pCollisionGrid = nullptr;
			original_map.rooms[room_index].pCollisionGrid = nullptr;

			// Call both implementations
			sut(nullptr, &moo_map.rooms[room_index]);
			original(nullptr, &original_map.rooms[room_index]);

			// Compare potentially modified input data
			D2RoomCollisionGridStrc* moo_pCollisionGrid = moo_map.rooms[room_index].pCollisionGrid;
			D2RoomCollisionGridStrc* original_pCollisionGrid = original_map.rooms[room_index].pCollisionGrid;
			REQUIRE(moo_pCollisionGrid != nullptr);
			REQUIRE(original_pCollisionGrid != nullptr);
			MOO_CHECK_EQ(moo_pCollisionGrid, original_pCollisionGrid, "Comparing pCollisionGrid");

			const int nCount = moo_pCollisionGrid->pRoomCoords.nSubtileWidth * moo_pCollisionGrid->pRoomCoords.nSubtileHeight;
			auto wrapped_moo_masks = DynamicArray<uint16_t>{ moo_pCollisionGrid->pCollisionMask, nCount };
			auto wrapped_original_masks = DynamicArray<uint16_t>{ original_pCollisionGrid->pCollisionMask, nCount };
			MOO_CHECK_EQ(wrapped_moo_masks, wrapped_original_masks, "Comparing collision masks");

			sut_free(nullptr, &moo_map.rooms[room_index]);
			original_free(nullptr, &original_map.rooms[room_index]);

			if (room_index != removed_room)
			{
				moo_map.rooms[room_index].pCollisionGrid = &moo_map.collision_grids[room_index];
				original_map.rooms[room_index].pCollisionGrid = &original_map.collision_grids[room_index];
			}

			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD413E0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD413E0, dll_base + 0x000013E0);

		REPEAT_10();

		const auto grid_index = GENERATE(center_room, 0, 8);
		const auto bRemoveOldFlags = GENERATE(0, 1);

		SUBCASE("")
		{
			constexpr int tile_count = 12;

			reroll_masks(50);

			D2TileLibraryEntryStrc moo_pTileLibraryEntries[4]{};
			D2TileLibraryEntryStrc original_pTileLibraryEntries[4]{};
			for (int i = 0; i < std::size(moo_pTileLibraryEntries); ++i)
			{
				randomize_tile_flags(moo_pTileLibraryEntries[i], original_pTileLibraryEntries[i]);
			}

			for (int nAdjacentRoom = 0; nAdjacentRoom < room_count; ++nAdjacentRoom)
			{
				// Input data
				D2DrlgTileDataStrc moo_pTiles[tile_count]{};
				D2DrlgTileDataStrc original_pTiles[tile_count]{};
				setup_random_tiles(moo_pTiles, original_pTiles, tile_count, moo_map.rooms[nAdjacentRoom].tCoords, moo_pTileLibraryEntries, original_pTileLibraryEntries, std::size(moo_pTileLibraryEntries));

				const int nTiles = random_unsigned_integer(0, tile_count);

				// Call both implementations
				sut(&moo_map.collision_grids[grid_index], &moo_map.collision_grids[nAdjacentRoom], moo_pTiles, nTiles, bRemoveOldFlags);
				original(&original_map.collision_grids[grid_index], &original_map.collision_grids[nAdjacentRoom], original_pTiles, nTiles, bRemoveOldFlags);

				// Compare potentially modified input data
				check_collision_maps(*this);
				for (int i = 0; i < tile_count; ++i)
				{
					MOO_CHECK_EQ(moo_pTiles[i], original_pTiles[i], "Comparing pTiles");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD41610")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_FreeRoomCollisionGrid, dll_base + 0x00001610);
		const auto [sut_alloc, original_alloc] = make_function_pair(COLLISION_AllocRoomCollisionGrid, dll_base + 0x000012C0);

		const auto room_index = GENERATE(center_room, 0, 8);

		SUBCASE("allocated collision grid")
		{
			D2DrlgRoomTilesStrc moo_pRoomTiles[room_count]{};
			D2DrlgRoomTilesStrc original_pRoomTiles[room_count]{};
			for (int i = 0; i < room_count; ++i)
			{
				moo_map.rooms[i].pRoomTiles = &moo_pRoomTiles[i];
				original_map.rooms[i].pRoomTiles = &original_pRoomTiles[i];
			}

			moo_map.rooms[room_index].pCollisionGrid = nullptr;
			original_map.rooms[room_index].pCollisionGrid = nullptr;
			sut_alloc(nullptr, &moo_map.rooms[room_index]);
			original_alloc(nullptr, &original_map.rooms[room_index]);
			REQUIRE(moo_map.rooms[room_index].pCollisionGrid != nullptr);
			REQUIRE(original_map.rooms[room_index].pCollisionGrid != nullptr);

			// Call both implementations
			sut(nullptr, &moo_map.rooms[room_index]);
			original(nullptr, &original_map.rooms[room_index]);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_map.rooms[room_index], original_map.rooms[room_index], "Comparing pRoom");

			CHECK_EQ(moo_map.rooms[room_index].pCollisionGrid, nullptr);
		}

		SUBCASE("no collision grid")
		{
			moo_map.rooms[room_index].pCollisionGrid = nullptr;
			original_map.rooms[room_index].pCollisionGrid = nullptr;

			// Call both implementations
			sut(nullptr, &moo_map.rooms[room_index]);
			original(nullptr, &original_map.rooms[room_index]);

			// Compare potentially modified input data
			check_collision_maps(*this);
		}

		SUBCASE("no room")
		{
			// Call both implementations
			sut(nullptr, nullptr);
			original(nullptr, nullptr);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD41650 (#10118)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckMask, dll_base + 0x00001650);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(30);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX, nY, nMask);
					const auto original_result = original(original_room(start_room), nX, nY, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD41720 (#10127)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetMask, dll_base + 0x00001720);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(30);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nMask);
					original(original_room(start_room), nX, nY, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD417F0 (#10123)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetMask, dll_base + 0x000017F0);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(70);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nMask);
					original(original_room(start_room), nX, nY, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD418C0 (#10120)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckMaskWithSizeXY, dll_base + 0x000018C0);

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nSizeX = GENERATE(0u, 1u, 2u, 3u, 7u, 0xFFFFFFFFu);
		const auto nSizeY = GENERATE(0u, 1u, 4u, 0xFFFFFFFFu);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX, nY, nSizeX, nSizeY, nMask);
					const auto original_result = original(original_room(start_room), nX, nY, nSizeX, nSizeY, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD41B40")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckCollisionMaskForBoundingBox, dll_base + 0x00001B40);

		REPEAT_5();

		SUBCASE("")
		{
			reroll_masks(10);

			for (int nRoom = 0; nRoom < room_count; ++nRoom)
			{
				const auto& tCoords = moo_map.collision_grids[nRoom].pRoomCoords;
				for (int nY = tCoords.nSubtileY; nY < tCoords.nSubtileY + tCoords.nSubtileHeight; ++nY)
				{
					for (int nX = tCoords.nSubtileX; nX < tCoords.nSubtileX + tCoords.nSubtileWidth; ++nX)
					{
						// Input data, the bounding box has to fit in the grid. Width and height may be 0.
						const int nWidth = random_unsigned_integer(0, tCoords.nSubtileX + tCoords.nSubtileWidth - nX);
						const int nHeight = random_unsigned_integer(0, tCoords.nSubtileY + tCoords.nSubtileHeight - nY);
						const uint16_t nMask = random_collision_mask();

						D2BoundingBoxStrc moo_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };
						D2BoundingBoxStrc original_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };

						// Call both implementations
						const auto moo_result = sut(&moo_map.collision_grids[nRoom], &moo_pBoundingBox, nMask);
						const auto original_result = original(&original_map.collision_grids[nRoom], &original_pBoundingBox, nMask);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

						// Compare potentially modified input data
						MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
					}
				}
			}

			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD41BE0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_AdaptBoundingBoxToGrid, dll_base + 0x00001BE0);

		REPEAT_10();

		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);

			for (int nRoom = 0; nRoom < room_count; ++nRoom)
			{
				const auto& tCoords = moo_map.rooms[nRoom].tCoords;
				for (int i = 0; i < 200; ++i)
				{
					// Input data, boxes may be inverted or stick out of the room on any side
					const int nLeft = random_signed_integer(tCoords.nSubtileX - 3, tCoords.nSubtileX + tCoords.nSubtileWidth + 3);
					const int nBottom = random_signed_integer(tCoords.nSubtileY - 3, tCoords.nSubtileY + tCoords.nSubtileHeight + 3);
					const int nRight = nLeft + random_signed_integer(-2, 30);
					const int nTop = nBottom + random_signed_integer(-2, 30);

					D2BoundingBoxStrc moo_pBoundingBox{ nLeft, nBottom, nRight, nTop };
					D2BoundingBoxStrc original_pBoundingBox{ nLeft, nBottom, nRight, nTop };

					D2BoundingBoxStrc moo_pBoundingBoxes[3]{};
					D2BoundingBoxStrc original_pBoundingBoxes[3]{};
					for (int j = 0; j < 3; ++j)
					{
						const D2BoundingBoxStrc tGarbage{ static_cast<int32_t>(random_unsigned_integer()), static_cast<int32_t>(random_unsigned_integer()), static_cast<int32_t>(random_unsigned_integer()), static_cast<int32_t>(random_unsigned_integer()) };
						moo_pBoundingBoxes[j] = tGarbage;
						original_pBoundingBoxes[j] = tGarbage;
					}

					// Call both implementations
					const auto moo_result = sut(&moo_map.rooms[nRoom], &moo_pBoundingBox, moo_pBoundingBoxes);
					const auto original_result = original(&original_map.rooms[nRoom], &original_pBoundingBox, original_pBoundingBoxes);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
					for (int j = 0; j < 3; ++j)
					{
						MOO_CHECK_EQ(moo_pBoundingBoxes[j], original_pBoundingBoxes[j], "Comparing pBoundingBoxes");
					}
				}
			}

			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD41CA0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckCollisionMaskForBoundingBoxRecursively, dll_base + 0x00001CA0);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(5);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					// Input data, boxes may span over several rooms or stick out of the map
					const int nWidth = random_unsigned_integer(0, 20);
					const int nHeight = random_unsigned_integer(0, 20);
					const uint16_t nMask = random_collision_mask();

					D2BoundingBoxStrc moo_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };
					D2BoundingBoxStrc original_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), &moo_pBoundingBox, nMask);
					const auto original_result = original(original_room(start_room), &original_pBoundingBox, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
				}
			}

			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD41DE0 (#10121)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckMaskWithPattern, dll_base + 0x00001DE0);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nCollisionPattern = GENERATE(-1, 0, 1, 2, 3, 4, 5, 6);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX, nY, nCollisionPattern, nMask);
					const auto original_result = original(original_room(start_room), nX, nY, nCollisionPattern, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD42000")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckCollisionMaskWithAdjacentCells, dll_base + 0x00002000);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX, nY, nMask);
					const auto original_result = original(original_room(start_room), nX, nY, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD42670")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckCollisionMask, dll_base + 0x00002670);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(30);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX, nY, nMask);
					const auto original_result = original(original_room(start_room), nX, nY, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD42740 (#10122)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckAnyCollisionWithPattern, dll_base + 0x00002740);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nCollisionPattern = GENERATE(-1, 0, 1, 2, 3, 4, 5, 6);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX, nY, nCollisionPattern, nMask);
					const auto original_result = original(original_room(start_room), nX, nY, nCollisionPattern, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD42A30")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckAnyCollisionWithAdjacentCells, dll_base + 0x00002A30);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX, nY, nMask);
					const auto original_result = original(original_room(start_room), nX, nY, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD43080 (#10119)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CheckMaskWithSize, dll_base + 0x00003080);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nUnitSize = GENERATE(-1, 0, 1, 2, 3, 4);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX, nY, nUnitSize, nMask);
					const auto original_result = original(original_room(start_room), nX, nY, nUnitSize, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD432A0 (#10128)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetMaskWithSize, dll_base + 0x000032A0);

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nUnitSize = GENERATE(-1, 0, 1, 2, 3, 4);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = static_cast<uint16_t>(1 << random_unsigned_integer(0, 15));

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nUnitSize, nMask);
					original(original_room(start_room), nX, nY, nUnitSize, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD434B0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetCollisionMask, dll_base + 0x000034B0);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(30);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nMask);
					original(original_room(start_room), nX, nY, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD43580")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetCollisionMaskForBoundingBoxRecursively, dll_base + 0x00003580);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					// Input data, boxes may span over several rooms or stick out of the map
					const int nWidth = random_unsigned_integer(0, 20);
					const int nHeight = random_unsigned_integer(0, 20);
					const uint16_t nMask = static_cast<uint16_t>(1 << random_unsigned_integer(0, 15));

					D2BoundingBoxStrc moo_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };
					D2BoundingBoxStrc original_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };

					// Call both implementations
					sut(moo_room(start_room), &moo_pBoundingBox, nMask);
					original(original_room(start_room), &original_pBoundingBox, nMask);

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD436F0 (#10130)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetMaskWithPattern, dll_base + 0x000036F0);

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nCollisionPattern = GENERATE(-1, 0, 1, 2, 3, 4, 5, 6);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					// Input data, 0 is special cased for some patterns
					const uint16_t nMask = random_unsigned_integer(0, 3) ? static_cast<uint16_t>(1 << random_unsigned_integer(0, 15)) : 0;

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nCollisionPattern, nMask);
					original(original_room(start_room), nX, nY, nCollisionPattern, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD439D0 (#10124)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetMaskWithSize, dll_base + 0x000039D0);

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nUnitSize = GENERATE(-1, 0, 1, 2, 3, 4);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(90);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = static_cast<uint16_t>(1 << random_unsigned_integer(0, 15));

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nUnitSize, nMask);
					original(original_room(start_room), nX, nY, nUnitSize, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD43C10")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetCollisionMask, dll_base + 0x00003C10);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(70);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nMask);
					original(original_room(start_room), nX, nY, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD43CE0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetCollisionMaskForBoundingBoxRecursively, dll_base + 0x00003CE0);

		REPEAT_5();

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(90);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					// Input data, boxes may span over several rooms or stick out of the map
					const int nWidth = random_unsigned_integer(0, 20);
					const int nHeight = random_unsigned_integer(0, 20);
					const uint16_t nMask = static_cast<uint16_t>(1 << random_unsigned_integer(0, 15));

					D2BoundingBoxStrc moo_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };
					D2BoundingBoxStrc original_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };

					// Call both implementations
					sut(moo_room(start_room), &moo_pBoundingBox, nMask);
					original(original_room(start_room), &original_pBoundingBox, nMask);

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD43E60 (#10126)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetMaskWithPattern, dll_base + 0x00003E60);

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nCollisionPattern = GENERATE(-1, 0, 1, 2, 3, 4, 5, 6);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(90);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					// Input data, 0 is special cased for some patterns
					const uint16_t nMask = random_unsigned_integer(0, 3) ? static_cast<uint16_t>(1 << random_unsigned_integer(0, 15)) : 0;

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nCollisionPattern, nMask);
					original(original_room(start_room), nX, nY, nCollisionPattern, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD44140 (#10125)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetMaskWithSizeXY, dll_base + 0x00004140);

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nSizeX = GENERATE(0u, 1u, 2u, 3u, 7u, 0xFFFFFFFFu);
		const auto nSizeY = GENERATE(0u, 1u, 4u, 0xFFFFFFFFu);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(90);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = static_cast<uint16_t>(1 << random_unsigned_integer(0, 15));

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nSizeX, nSizeY, nMask);
					original(original_room(start_room), nX, nY, nSizeX, nSizeY, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD44370")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ResetCollisionMaskForBoundingBox, dll_base + 0x00004370);

		REPEAT_5();

		SUBCASE("")
		{
			reroll_masks(90);

			for (int nRoom = 0; nRoom < room_count; ++nRoom)
			{
				const auto& tCoords = moo_map.collision_grids[nRoom].pRoomCoords;
				for (int nY = tCoords.nSubtileY; nY < tCoords.nSubtileY + tCoords.nSubtileHeight; ++nY)
				{
					for (int nX = tCoords.nSubtileX; nX < tCoords.nSubtileX + tCoords.nSubtileWidth; ++nX)
					{
						// Input data, the bounding box has to fit in the grid. Width and height may be 0.
						const int nWidth = random_unsigned_integer(0, tCoords.nSubtileX + tCoords.nSubtileWidth - nX);
						const int nHeight = random_unsigned_integer(0, tCoords.nSubtileY + tCoords.nSubtileHeight - nY);
						const uint16_t nMask = static_cast<uint16_t>(1 << random_unsigned_integer(0, 15));

						D2BoundingBoxStrc moo_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };
						D2BoundingBoxStrc original_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };

						// Call both implementations
						sut(&moo_map.collision_grids[nRoom], &moo_pBoundingBox, nMask);
						original(&original_map.collision_grids[nRoom], &original_pBoundingBox, nMask);

						// Compare potentially modified input data
						MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
						check_collision_maps(*this);
					}
				}
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD443E0 (#10129)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetMaskWithSizeXY, dll_base + 0x000043E0);

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nSizeX = GENERATE(0u, 1u, 2u, 3u, 7u, 0xFFFFFFFFu);
		const auto nSizeY = GENERATE(0u, 1u, 4u, 0xFFFFFFFFu);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					const uint16_t nMask = static_cast<uint16_t>(1 << random_unsigned_integer(0, 15));

					// Call both implementations
					sut(moo_room(start_room), nX, nY, nSizeX, nSizeY, nMask);
					original(original_room(start_room), nX, nY, nSizeX, nSizeY, nMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD44600")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetCollisionMaskForBoundingBox, dll_base + 0x00004600);

		REPEAT_5();

		SUBCASE("")
		{
			reroll_masks(10);

			for (int nRoom = 0; nRoom < room_count; ++nRoom)
			{
				const auto& tCoords = moo_map.collision_grids[nRoom].pRoomCoords;
				for (int nY = tCoords.nSubtileY; nY < tCoords.nSubtileY + tCoords.nSubtileHeight; ++nY)
				{
					for (int nX = tCoords.nSubtileX; nX < tCoords.nSubtileX + tCoords.nSubtileWidth; ++nX)
					{
						// Input data, the bounding box has to fit in the grid. Width and height may be 0.
						const int nWidth = random_unsigned_integer(0, tCoords.nSubtileX + tCoords.nSubtileWidth - nX);
						const int nHeight = random_unsigned_integer(0, tCoords.nSubtileY + tCoords.nSubtileHeight - nY);
						const uint16_t nMask = static_cast<uint16_t>(1 << random_unsigned_integer(0, 15));

						D2BoundingBoxStrc moo_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };
						D2BoundingBoxStrc original_pBoundingBox{ nX, nY, nX + nWidth - 1, nY + nHeight - 1 };

						// Call both implementations
						sut(&moo_map.collision_grids[nRoom], &moo_pBoundingBox, nMask);
						original(&original_map.collision_grids[nRoom], &original_pBoundingBox, nMask);

						// Compare potentially modified input data
						MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
						check_collision_maps(*this);
					}
				}
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD44660 (#10131)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_TryMoveUnitCollisionMask, dll_base + 0x00004660);

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nUnitSize = GENERATE(-1, 0, 1, 2, 3, 4);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY1 = origin_y - 2; nY1 < origin_y + map_height + 2; ++nY1)
			{
				for (int nX1 = origin_x - 2; nX1 < origin_x + map_width + 2; ++nX1)
				{
					// Input data, units move to one of the neighbouring subtiles
					const int nX2 = nX1 + random_signed_integer(-2, 2);
					const int nY2 = nY1 + random_signed_integer(-2, 2);
					const uint16_t nCollisionMask = random_unsigned_integer(0, 1) ? COLLIDE_MONSTER : COLLIDE_PLAYER;
					const uint16_t nMoveConditionMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX1, nY1, nX2, nY2, nUnitSize, nCollisionMask, nMoveConditionMask);
					const auto original_result = original(original_room(start_room), nX1, nY1, nX2, nY2, nUnitSize, nCollisionMask, nMoveConditionMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD44910")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_CreateBoundingBox, dll_base + 0x00004910);

		SUBCASE("")
		{
			for (int i = 0; i < 10000; ++i)
			{
				// Input data
				D2BoundingBoxStrc moo_pBoundingBox{};
				D2BoundingBoxStrc original_pBoundingBox{};
				const int nCenterX = static_cast<int>(random_unsigned_integer());
				const int nCenterY = static_cast<int>(random_unsigned_integer());
				const unsigned int nSizeX = random_unsigned_integer(0, 3) ? random_unsigned_integer(0, 10) : random_unsigned_integer();
				const unsigned int nSizeY = random_unsigned_integer(0, 3) ? random_unsigned_integer(0, 10) : random_unsigned_integer();

				// Call both implementations
				sut(&moo_pBoundingBox, nCenterX, nCenterY, nSizeX, nSizeY);
				original(&original_pBoundingBox, nCenterX, nCenterY, nSizeX, nSizeY);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");
			}
		}

		SUBCASE("3x3")
		{
			D2BoundingBoxStrc moo_pBoundingBox{};
			D2BoundingBoxStrc original_pBoundingBox{};

			// Call both implementations
			sut(&moo_pBoundingBox, 100, -20, 3, 3);
			original(&original_pBoundingBox, 100, -20, 3, 3);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pBoundingBox, original_pBoundingBox, "Comparing pBoundingBox");

			CHECK_EQ(moo_pBoundingBox.nLeft, 99);
			CHECK_EQ(moo_pBoundingBox.nBottom, -21);
			CHECK_EQ(moo_pBoundingBox.nRight, 101);
			CHECK_EQ(moo_pBoundingBox.nTop, -19);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD44950 (#10132)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_TryTeleportUnitCollisionMask, dll_base + 0x00004950);

		const auto start_room = GENERATE(no_room, center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nCollisionPattern = GENERATE(-1, 0, 1, 2, 3, 4, 5, 6);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY1 = origin_y - 2; nY1 < origin_y + map_height + 2; ++nY1)
			{
				for (int nX1 = origin_x - 2; nX1 < origin_x + map_width + 2; ++nX1)
				{
					// Input data, units can teleport anywhere in the map
					const int nX2 = random_signed_integer(origin_x - 2, origin_x + map_width + 1);
					const int nY2 = random_signed_integer(origin_y - 2, origin_y + map_height + 1);
					const uint16_t nFootprintCollisionMask = random_unsigned_integer(0, 1) ? COLLIDE_MONSTER : COLLIDE_PLAYER;
					const uint16_t nMoveConditionMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX1, nY1, nX2, nY2, nCollisionPattern, nFootprintCollisionMask, nMoveConditionMask);
					const auto original_result = original(original_room(start_room), nX1, nY1, nX2, nY2, nCollisionPattern, nFootprintCollisionMask, nMoveConditionMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD44BB0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_ForceTeleportUnitCollisionMaskAndGetCollision, dll_base + 0x00004BB0);

		const auto start_room1 = GENERATE(no_room, center_room, 0, 1);
		const auto start_room2 = GENERATE(no_room, center_room, 8);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nUnitSize = GENERATE(-1, 0, 1, 2, 3, 4);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY1 = origin_y - 2; nY1 < origin_y + map_height + 2; ++nY1)
			{
				for (int nX1 = origin_x - 2; nX1 < origin_x + map_width + 2; ++nX1)
				{
					// Input data, units can teleport anywhere in the map
					const int nX2 = random_signed_integer(origin_x - 2, origin_x + map_width + 1);
					const int nY2 = random_signed_integer(origin_y - 2, origin_y + map_height + 1);
					const uint16_t nFootprintCollisionMask = random_unsigned_integer(0, 1) ? COLLIDE_MONSTER : COLLIDE_PLAYER;
					const uint16_t nMoveConditionMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room1), nX1, nY1, moo_room(start_room2), nX2, nY2, nUnitSize, nFootprintCollisionMask, nMoveConditionMask);
					const auto original_result = original(original_room(start_room1), nX1, nY1, original_room(start_room2), nX2, nY2, nUnitSize, nFootprintCollisionMask, nMoveConditionMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD44E00")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_TeleportUnitCollisionMask, dll_base + 0x00004E00);

		const auto start_room1 = GENERATE(no_room, center_room, 0, 1);
		const auto start_room2 = GENERATE(no_room, center_room, 8);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nUnitSize = GENERATE(-1, 0, 1, 2, 3, 4);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY1 = origin_y - 2; nY1 < origin_y + map_height + 2; ++nY1)
			{
				for (int nX1 = origin_x - 2; nX1 < origin_x + map_width + 2; ++nX1)
				{
					// Input data, units can teleport anywhere in the map
					const int nX2 = random_signed_integer(origin_x - 2, origin_x + map_width + 1);
					const int nY2 = random_signed_integer(origin_y - 2, origin_y + map_height + 1);
					const uint16_t nMask = random_unsigned_integer(0, 1) ? COLLIDE_MONSTER : COLLIDE_PLAYER;

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room1), nX1, nY1, moo_room(start_room2), nX2, nY2, nUnitSize, nMask);
					const auto original_result = original(original_room(start_room1), nX1, nY1, original_room(start_room2), nX2, nY2, nUnitSize, nMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD44FF0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_TrySetUnitCollisionMask, dll_base + 0x00004FF0);

		const auto start_room1 = GENERATE(no_room, center_room, 0, 1);
		const auto start_room2 = GENERATE(no_room, center_room, 8);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nCollisionPattern = GENERATE(-1, 0, 1, 2, 3, 4, 5, 6);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(5);

			for (int nY1 = origin_y - 2; nY1 < origin_y + map_height + 2; ++nY1)
			{
				for (int nX1 = origin_x - 2; nX1 < origin_x + map_width + 2; ++nX1)
				{
					// Input data, units can teleport anywhere in the map
					const int nX2 = random_signed_integer(origin_x - 2, origin_x + map_width + 1);
					const int nY2 = random_signed_integer(origin_y - 2, origin_y + map_height + 1);
					const uint16_t nFootprintCollisionMask = random_unsigned_integer(0, 1) ? COLLIDE_MONSTER : COLLIDE_PLAYER;
					const uint16_t nMoveConditionMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room1), nX1, nY1, moo_room(start_room2), nX2, nY2, nCollisionPattern, nFootprintCollisionMask, nMoveConditionMask);
					const auto original_result = original(original_room(start_room1), nX1, nY1, original_room(start_room2), nX2, nY2, nCollisionPattern, nFootprintCollisionMask, nMoveConditionMask);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD451D0 (#10133)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_SetUnitCollisionMask, dll_base + 0x000051D0);

		const auto start_room1 = GENERATE(no_room, center_room, 0, 1);
		const auto start_room2 = GENERATE(no_room, center_room, 8);
		const auto removed_room = GENERATE(no_room, 1);
		const auto nCollisionPattern = GENERATE(-1, 0, 1, 2, 3, 4, 5, 6);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(10);

			for (int nY1 = origin_y - 2; nY1 < origin_y + map_height + 2; ++nY1)
			{
				for (int nX1 = origin_x - 2; nX1 < origin_x + map_width + 2; ++nX1)
				{
					// Input data, units can teleport anywhere in the map
					const int nX2 = random_signed_integer(origin_x - 2, origin_x + map_width + 1);
					const int nY2 = random_signed_integer(origin_y - 2, origin_y + map_height + 1);
					const uint16_t nCollisionMask = random_unsigned_integer(0, 3) ? static_cast<uint16_t>(1 << random_unsigned_integer(0, 15)) : 0;

					// Call both implementations
					sut(moo_room(start_room1), nX1, nY1, moo_room(start_room2), nX2, nY2, nCollisionPattern, nCollisionMask);
					original(original_room(start_room1), nX1, nY1, original_room(start_room2), nX2, nY2, nCollisionPattern, nCollisionMask);
				}

				// Compare potentially modified input data, once per row to keep the test fast
				check_collision_maps(*this);
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD459D0 (#10135)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinatesWithMaxDistance, dll_base + 0x000059D0);

		const auto start_room = GENERATE(no_room, center_room, 0, 8);
		const auto removed_room = GENERATE(no_room, 1);
		const auto density = GENERATE(20u, 80u, 98u);
		const auto nMaxDistance = GENERATE(-1, 0, 1, 2, 7, 50);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(density);

			for (int nY = origin_y - 3; nY < origin_y + map_height + 3; nY += 3)
			{
				for (int nX = origin_x - 3; nX < origin_x + map_width + 3; nX += 3)
				{
					// Input data
					D2CoordStrc moo_pSpawnPoint{ nX, nY };
					D2CoordStrc original_pSpawnPoint{ nX, nY };
					const int nUnitSize = random_unsigned_integer(0, 3);
					const unsigned int nMask = random_collision_mask();
					const BOOL bAllowNeighborRooms = random_unsigned_integer(0, 1);

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), &moo_pSpawnPoint, nUnitSize, nMask, bAllowNeighborRooms, nMaxDistance);
					const auto original_result = original(original_room(start_room), &original_pSpawnPoint, nUnitSize, nMask, bAllowNeighborRooms, nMaxDistance);

					// Compare return values
					CHECK_EQ(moo_map.index_of(moo_result), original_map.index_of(original_result));

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
				}
			}

			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionWithFieldFixture, "D2Common.0x6FD45A00")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinatesImpl, dll_base + 0x00005A00);

		const auto start_room = GENERATE(no_room, center_room, 0, 8);
		const auto removed_room = GENERATE(no_room, 1);
		const auto density = GENERATE(20u, 80u, 98u);
		const auto has_field = GENERATE(0, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(density);

			for (int nY = origin_y - 3; nY < origin_y + map_height + 3; nY += 3)
			{
				for (int nX = origin_x - 3; nX < origin_x + map_width + 3; nX += 3)
				{
					// Input data
					D2CoordStrc moo_ptSpawnPoint{ nX, nY };
					D2CoordStrc original_ptSpawnPoint{ nX, nY };
					D2CoordStrc moo_pFieldCoord{ nX + random_signed_integer(-20, 20), nY + random_signed_integer(-20, 20) };
					D2CoordStrc original_pFieldCoord = moo_pFieldCoord;
					const int nUnitSize = random_unsigned_integer(0, 3);
					const unsigned int nMask = random_collision_mask();
					const unsigned int nFieldMask = random_collision_mask();
					const BOOL bAllowNeighborRooms = random_unsigned_integer(0, 1);
					const int nMaxDistance = random_signed_integer(-1, 50);
					const int nPosIncrementValue = random_signed_integer(1, 3);

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), &moo_ptSpawnPoint, has_field ? &moo_pFieldCoord : nullptr, nUnitSize, nMask, nFieldMask, bAllowNeighborRooms, nMaxDistance, nPosIncrementValue);
					const auto original_result = original(original_room(start_room), &original_ptSpawnPoint, has_field ? &original_pFieldCoord : nullptr, nUnitSize, nMask, nFieldMask, bAllowNeighborRooms, nMaxDistance, nPosIncrementValue);

					// Compare return values
					CHECK_EQ(moo_map.index_of(moo_result), original_map.index_of(original_result));

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_ptSpawnPoint, original_ptSpawnPoint, "Comparing ptSpawnPoint");
					MOO_CHECK_EQ(moo_pFieldCoord, original_pFieldCoord, "Comparing pFieldCoord");
				}
			}

			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD46280 (#10134)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinates, dll_base + 0x00006280);

		const auto start_room = GENERATE(no_room, center_room, 0, 8);
		const auto removed_room = GENERATE(no_room, 1);
		const auto density = GENERATE(20u, 80u, 98u);
		const auto bAllowNeighborRooms = GENERATE(0, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(density);

			for (int nY = origin_y - 3; nY < origin_y + map_height + 3; nY += 3)
			{
				for (int nX = origin_x - 3; nX < origin_x + map_width + 3; nX += 3)
				{
					// Input data
					D2CoordStrc moo_pSpawnPoint{ nX, nY };
					D2CoordStrc original_pSpawnPoint{ nX, nY };
					const int nUnitSize = random_unsigned_integer(0, 3);
					const unsigned int nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), &moo_pSpawnPoint, nUnitSize, nMask, bAllowNeighborRooms);
					const auto original_result = original(original_room(start_room), &original_pSpawnPoint, nUnitSize, nMask, bAllowNeighborRooms);

					// Compare return values
					CHECK_EQ(moo_map.index_of(moo_result), original_map.index_of(original_result));

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
				}
			}

			check_collision_maps(*this);
		}

		SUBCASE("Free subtile around the maximum search distance")
		{
			build_maps(no_room);

			const int nSpawnX = origin_x + 1;
			const int nSpawnY = origin_y + 1;
			for (int nOffsetY = 0; nOffsetY <= 20; nOffsetY += 20)
			{
				for (int nDistance = 45; nDistance <= 55; ++nDistance)
				{
					block_all_but(nSpawnX + nDistance, nSpawnY + nOffsetY);

					for (int nUnitSize = 0; nUnitSize <= 1; ++nUnitSize)
					{
						// Input data
						D2CoordStrc moo_pSpawnPoint{ nSpawnX, nSpawnY };
						D2CoordStrc original_pSpawnPoint{ nSpawnX, nSpawnY };

						// Call both implementations
						const auto moo_result = sut(moo_room(0), &moo_pSpawnPoint, nUnitSize, COLLIDE_WALL, bAllowNeighborRooms);
						const auto original_result = original(original_room(0), &original_pSpawnPoint, nUnitSize, COLLIDE_WALL, bAllowNeighborRooms);

						// Compare return values
						CHECK_EQ(moo_map.index_of(moo_result), original_map.index_of(original_result));

						// Compare potentially modified input data
						MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
					}
				}
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD462B0 (#10137)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinatesEx, dll_base + 0x000062B0);

		const auto start_room = GENERATE(no_room, center_room, 0, 8);
		const auto removed_room = GENERATE(no_room, 1);
		const auto density = GENERATE(20u, 80u, 98u);
		// NOTE: 0 or negative values never end
		const auto nPosIncrementValue = GENERATE(1, 2, 3);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(density);

			for (int nY = origin_y - 3; nY < origin_y + map_height + 3; nY += 3)
			{
				for (int nX = origin_x - 3; nX < origin_x + map_width + 3; nX += 3)
				{
					// Input data
					D2CoordStrc moo_pSpawnPoint{ nX, nY };
					D2CoordStrc original_pSpawnPoint{ nX, nY };
					const int nUnitSize = random_unsigned_integer(0, 3);
					const unsigned int nMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), &moo_pSpawnPoint, nUnitSize, nMask, nPosIncrementValue);
					const auto original_result = original(original_room(start_room), &original_pSpawnPoint, nUnitSize, nMask, nPosIncrementValue);

					// Compare return values
					CHECK_EQ(moo_map.index_of(moo_result), original_map.index_of(original_result));

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
				}
			}

			check_collision_maps(*this);
		}

		SUBCASE("Free subtile around the maximum search distance")
		{
			build_maps(no_room);

			const int nSpawnX = origin_x + 1;
			const int nSpawnY = origin_y + 1;
			for (int nOffsetY = 0; nOffsetY <= 20; nOffsetY += 20)
			{
				for (int nDistance = 45; nDistance <= 55; ++nDistance)
				{
					block_all_but(nSpawnX + nDistance, nSpawnY + nOffsetY);

					for (int nUnitSize = 0; nUnitSize <= 1; ++nUnitSize)
					{
						// Input data
						D2CoordStrc moo_pSpawnPoint{ nSpawnX, nSpawnY };
						D2CoordStrc original_pSpawnPoint{ nSpawnX, nSpawnY };

						// Call both implementations
						const auto moo_result = sut(moo_room(0), &moo_pSpawnPoint, nUnitSize, COLLIDE_WALL, nPosIncrementValue);
						const auto original_result = original(original_room(0), &original_pSpawnPoint, nUnitSize, COLLIDE_WALL, nPosIncrementValue);

						// Compare return values
						CHECK_EQ(moo_map.index_of(moo_result), original_map.index_of(original_result));

						// Compare potentially modified input data
						MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
					}
				}
			}
		}
	}

	TEST_CASE_FIXTURE(CollisionWithFieldFixture, "D2Common.0x6FD462E0 (#10138)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetFreeCoordinatesWithField, dll_base + 0x000062E0);

		const auto start_room = GENERATE(no_room, center_room, 0, 8);
		const auto removed_room = GENERATE(no_room, 1);
		const auto density = GENERATE(5u, 20u, 80u, 98u);
		const auto bAllowNeighborRooms = GENERATE(0, 1);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(density);

			for (int nY = origin_y - 3; nY < origin_y + map_height + 3; nY += 3)
			{
				for (int nX = origin_x - 3; nX < origin_x + map_width + 3; nX += 3)
				{
					// Input data
					D2CoordStrc moo_pSpawnPoint{ nX, nY };
					D2CoordStrc original_pSpawnPoint{ nX, nY };
					D2CoordStrc moo_pFieldCoord{ nX + random_signed_integer(-20, 20), nY + random_signed_integer(-20, 20) };
					D2CoordStrc original_pFieldCoord = moo_pFieldCoord;
					const int nUnitSize = random_unsigned_integer(0, 3);
					const unsigned int nMask = random_collision_mask();
					const unsigned int nFieldMask = random_collision_mask();

					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), &moo_pSpawnPoint, &moo_pFieldCoord, nUnitSize, nMask, nFieldMask, bAllowNeighborRooms);
					const auto original_result = original(original_room(start_room), &original_pSpawnPoint, &original_pFieldCoord, nUnitSize, nMask, nFieldMask, bAllowNeighborRooms);

					// Compare return values
					CHECK_EQ(moo_map.index_of(moo_result), original_map.index_of(original_result));

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pSpawnPoint, original_pSpawnPoint, "Comparing pSpawnPoint");
					MOO_CHECK_EQ(moo_pFieldCoord, original_pFieldCoord, "Comparing pFieldCoord");
				}
			}

			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD46310 (#10136)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10136, dll_base + 0x00006310);

		const auto start_room = GENERATE(center_room, 0, 8, 1);
		const auto removed_room = GENERATE(no_room, 1);
		const auto density = GENERATE(5u, 30u, 95u);
		const auto a3 = GENERATE(0, 1, 2);

		SUBCASE("")
		{
			build_maps(removed_room);
			reroll_masks(density);

			for (int nY = origin_y - 3; nY < origin_y + map_height + 3; nY += 4)
			{
				for (int nX = origin_x - 3; nX < origin_x + map_width + 3; nX += 4)
				{
					// Input data
					D2CoordStrc moo_pCoord{ nX, nY };
					D2CoordStrc original_pCoord{ nX, nY };
					D2ActiveRoomStrc* moo_ppRoom = &moo_map.rooms[center_room];
					D2ActiveRoomStrc* original_ppRoom = &original_map.rooms[center_room];
					const uint16_t nMask = random_collision_mask();

					// Call both implementations
					sut(moo_room(start_room), &moo_pCoord, a3, nMask, &moo_ppRoom);
					original(original_room(start_room), &original_pCoord, a3, nMask, &original_ppRoom);

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pCoord, original_pCoord, "Comparing pCoord");
					CHECK_EQ(moo_map.index_of(moo_ppRoom), original_map.index_of(original_ppRoom));
				}
			}

			check_collision_maps(*this);
		}
	}

	TEST_CASE_FIXTURE(CollisionFixture, "D2Common.0x6FD46620")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COLLISION_GetRoomBySubTileCoordinates, dll_base + 0x00006620);

		const auto start_room = GENERATE(no_room, center_room, 0, 2, 6, 8, 1);
		const auto removed_room = GENERATE(no_room, 1, 3);

		SUBCASE("")
		{
			build_maps(removed_room);

			for (int nY = origin_y - 2; nY < origin_y + map_height + 2; ++nY)
			{
				for (int nX = origin_x - 2; nX < origin_x + map_width + 2; ++nX)
				{
					// Call both implementations
					const auto moo_result = sut(moo_room(start_room), nX, nY);
					const auto original_result = original(original_room(start_room), nX, nY);

					// Compare return values
					CHECK_EQ(moo_map.index_of(moo_result), original_map.index_of(original_result));
				}
			}

			// Compare potentially modified input data
			check_collision_maps(*this);
		}
	}
}
#endif
