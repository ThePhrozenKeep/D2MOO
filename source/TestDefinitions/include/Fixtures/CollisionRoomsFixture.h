#pragma once


#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <vector>

#include <Windows.h>

#include <D2Collision.h>
#include <D2DataTbls.h>
#include <Drlg/D2DrlgDrlg.h>


// Builds two identical small maps (one for D2MOO, one for the original game) made of a 3x3 grid of
// adjacent active rooms of different sizes, each with its own collision grid.
// Every room lists itself and the rooms around it as adjacent rooms, like the game does.
template<class Fixture>
struct CollisionRoomsFixture : Fixture
{
	static constexpr int column_count = 3;
	static constexpr int row_count = 3;
	static constexpr int room_count = column_count * row_count;
	static constexpr int center_room = 4;
	static constexpr int no_room = -1;
	static constexpr int unknown_room = -2;

	// Subtile coordinates are multiples of 5 so the rooms are aligned on game tiles
	// The map is more than 50 subtiles wide so the maximum search distance of the free coordinates functions is reachable
	static constexpr int origin_x = 5000;
	static constexpr int origin_y = 3000;
	static constexpr int column_widths[column_count] = { 10, 45, 5 };
	static constexpr int row_heights[row_count] = { 15, 5, 10 };
	static constexpr int map_width = 10 + 45 + 5;
	static constexpr int map_height = 15 + 5 + 10;

	struct CollisionMap
	{
		D2ActiveRoomStrc rooms[room_count]{};
		D2RoomCollisionGridStrc collision_grids[room_count]{};
		std::vector<uint16_t> collision_masks[room_count];
		std::vector<D2ActiveRoomStrc*> adjacent_rooms[room_count];

		int index_of(const D2ActiveRoomStrc* pRoom) const
		{
			if (pRoom == nullptr)
			{
				return no_room;
			}

			for (int i = 0; i < room_count; ++i)
			{
				if (pRoom == &rooms[i])
				{
					return i;
				}
			}

			return unknown_room;
		}
	};

	CollisionMap moo_map;
	CollisionMap original_map;
	int missing_room = no_room;

	CollisionRoomsFixture()
	{
		build_maps(no_room);
	}

	// Rebuilds both maps. The missing room (if any) is not linked to the other rooms and has no collision grid.
	void build_maps(int nMissingRoom)
	{
		missing_room = nMissingRoom;

		const auto setup_data = [this](
			CollisionMap& map
		) {
			int nSubtileY = origin_y;
			for (int nRow = 0; nRow < row_count; ++nRow)
			{
				int nSubtileX = origin_x;
				for (int nColumn = 0; nColumn < column_count; ++nColumn)
				{
					const int i = nRow * column_count + nColumn;

					D2ActiveRoomStrc& pRoom = map.rooms[i];
					pRoom = {};
					pRoom.tCoords.nSubtileX = nSubtileX;
					pRoom.tCoords.nSubtileY = nSubtileY;
					pRoom.tCoords.nSubtileWidth = column_widths[nColumn];
					pRoom.tCoords.nSubtileHeight = row_heights[nRow];
					pRoom.tCoords.nTileXPos = nSubtileX / 5;
					pRoom.tCoords.nTileYPos = nSubtileY / 5;
					pRoom.tCoords.nTileWidth = column_widths[nColumn] / 5;
					pRoom.tCoords.nTileHeight = row_heights[nRow] / 5;

					map.collision_grids[i] = {};
					map.collision_grids[i].pRoomCoords = pRoom.tCoords;
					map.collision_masks[i].assign(column_widths[nColumn] * row_heights[nRow], 0);
					map.collision_grids[i].pCollisionMask = map.collision_masks[i].data();

					if (i != missing_room)
					{
						pRoom.pCollisionGrid = &map.collision_grids[i];
					}

					nSubtileX += column_widths[nColumn];
				}
				nSubtileY += row_heights[nRow];
			}

			for (int i = 0; i < room_count; ++i)
			{
				map.adjacent_rooms[i].clear();
				if (i == missing_room)
				{
					continue;
				}

				map.adjacent_rooms[i].push_back(&map.rooms[i]);
				for (int j = 0; j < room_count; ++j)
				{
					const int nColumnDistance = std::abs(i % column_count - j % column_count);
					const int nRowDistance = std::abs(i / column_count - j / column_count);
					if (j != i && j != missing_room && nColumnDistance <= 1 && nRowDistance <= 1)
					{
						map.adjacent_rooms[i].push_back(&map.rooms[j]);
					}
				}

				map.rooms[i].ppRoomList = map.adjacent_rooms[i].data();
				map.rooms[i].nNumRooms = static_cast<int32_t>(map.adjacent_rooms[i].size());
			}
		};

		setup_data(moo_map);
		setup_data(original_map);
	}

	// Fills the collision masks of both maps with the same random values.
	// nDensity is the chance (in percent) for a subtile to have any collision flag.
	void reroll_masks(uint32_t nDensity)
	{
		for (int i = 0; i < room_count; ++i)
		{
			for (size_t j = 0; j < moo_map.collision_masks[i].size(); ++j)
			{
				uint16_t nMask = 0;
				if (random_unsigned_integer(0, 99) < nDensity)
				{
					nMask = static_cast<uint16_t>(random_unsigned_integer(1, 0xFFFF));
				}

				moo_map.collision_masks[i][j] = nMask;
				original_map.collision_masks[i][j] = nMask;
			}
		}
	}

	// Blocks every subtile of both maps with all collision flags, except the subtile at nX, nY
	void block_all_but(int nX, int nY)
	{
		for (int i = 0; i < room_count; ++i)
		{
			const D2DrlgCoordsStrc& tCoords = moo_map.rooms[i].tCoords;
			for (int j = 0; j < tCoords.nSubtileWidth * tCoords.nSubtileHeight; ++j)
			{
				const bool bFree = tCoords.nSubtileX + j % tCoords.nSubtileWidth == nX && tCoords.nSubtileY + j / tCoords.nSubtileWidth == nY;
				moo_map.collision_masks[i][j] = bFree ? 0 : 0xFFFF;
				original_map.collision_masks[i][j] = bFree ? 0 : 0xFFFF;
			}
		}
	}

	D2ActiveRoomStrc* moo_room(int nRoom)
	{
		return nRoom == no_room ? nullptr : &moo_map.rooms[nRoom];
	}

	D2ActiveRoomStrc* original_room(int nRoom)
	{
		return nRoom == no_room ? nullptr : &original_map.rooms[nRoom];
	}
};


// Installs the same synthetic collision field (normally loaded from expfield.d2) in D2MOO and the original game.
// Each cell of the field points to its neighbour towards the center of the field, the center itself ends the walk.
// The field is not square, so that mixing up the center coordinates is noticed. It stays 256 wide because the game indexes its rows with a fixed width of 256.
template<class Fixture>
struct CollisionFieldFixture : Fixture
{
	static constexpr int field_width = 256;
	static constexpr int field_height = 200;
	static constexpr int field_center_x = field_width / 2;
	static constexpr int field_center_y = field_height / 2;

	std::unique_ptr<uint8_t[]> field_data;

	uint8_t* previous_field_data;
	D2CollisionFieldStrc previous_collision_field;

	uint8_t** original_field_data;
	D2CollisionFieldStrc* original_collision_field;
	uint8_t* previous_original_field_data;
	D2CollisionFieldStrc previous_original_collision_field;

	CollisionFieldFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		// The original game reads the field from its static data tables, not through sgptDataTables
		original_field_data = reinterpret_cast<uint8_t**>(d2common_base + 0x000A9608 + 0x00000CAC);
		original_collision_field = reinterpret_cast<D2CollisionFieldStrc*>(d2common_base + 0x000A9608 + 0x00000CB0);

		constexpr uint8_t directions[3][3] = {
			// dx = -1, 0, 1
			{ 7, 0, 1 }, // dy = -1
			{ 6, 8, 2 }, // dy = 0
			{ 5, 4, 3 }, // dy = 1
		};

		field_data = std::make_unique<uint8_t[]>(field_width * field_height);
		for (int y = 0; y < field_height; ++y)
		{
			for (int x = 0; x < field_width; ++x)
			{
				const int nStepX = (x < field_center_x) - (x > field_center_x);
				const int nStepY = (y < field_center_y) - (y > field_center_y);
				field_data[y * field_width + x] = directions[nStepY + 1][nStepX + 1];
			}
		}

		D2CollisionFieldStrc collision_field{};
		collision_field.nWidth = field_width;
		collision_field.nHeight = field_height;
		collision_field.nArea = field_width * field_height;
		collision_field.nCenterX = field_center_x;
		collision_field.nCenterY = field_center_y;

		previous_field_data = sgptDataTables->pFieldData;
		previous_collision_field = sgptDataTables->pCollisionField;
		sgptDataTables->pFieldData = field_data.get();
		sgptDataTables->pCollisionField = collision_field;

		previous_original_field_data = *original_field_data;
		previous_original_collision_field = *original_collision_field;
		*original_field_data = field_data.get();
		*original_collision_field = collision_field;
	}

	~CollisionFieldFixture()
	{
		sgptDataTables->pFieldData = previous_field_data;
		sgptDataTables->pCollisionField = previous_collision_field;

		*original_field_data = previous_original_field_data;
		*original_collision_field = previous_original_collision_field;
	}
};
