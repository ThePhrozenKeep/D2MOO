#include <D2CommonTestDefines.h>

#ifdef UNITROOM_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>
#include <vector>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlg.h>
#include <Units/UnitRoom.h>
#include <Units/Units.h>

#include <Fixtures/DataTbls/Fixtures.h>
#include <Fixtures/Units/UnitRoomScene.h>


namespace
{
	struct UnitKind
	{
		int nUnitType;
		int nClassId;
		int nAlignment;
	};

	// Covers every unit type and every branch of the allied unit counting
	const UnitKind unit_kinds[] = {
		{ UNIT_PLAYER, 0, -1 },
		{ UNIT_PLAYER, 3, UNIT_ALIGNMENT_EVIL },
		{ UNIT_MONSTER, 5, -1 },
		{ UNIT_MONSTER, 5, UNIT_ALIGNMENT_EVIL },
		{ UNIT_MONSTER, 150, UNIT_ALIGNMENT_NEUTRAL },
		{ UNIT_MONSTER, 271, UNIT_ALIGNMENT_GOOD },
		{ UNIT_OBJECT, 1, -1 },
		{ UNIT_OBJECT, 2, UNIT_ALIGNMENT_GOOD },
		{ UNIT_MISSILE, 10, -1 },
		{ UNIT_ITEM, 20, -1 },
		{ UNIT_TILE, 0, -1 },
	};
	constexpr int unit_kind_count = sizeof(unit_kinds) / sizeof(unit_kinds[0]);

	// Room 2 has dwFlags & 4 set, which disables its update list
	const std::vector<UnitRoomSceneRoomDesc> test_rooms = {
		{ 100, 100, 40, 40, -1, 0, { 0, 1, 2 } },
		{ 140, 100, 40, 40, -1, 0, { 1, 0 } },
		{ 100, 140, 40, 40, -1, 4, { 2, 0 } },
	};

	UnitRoomSceneUnitDesc make_unit(const UnitKind& kind, int nRoom, int nOffset, uint32_t dwFlags = 0)
	{
		const auto& room = test_rooms[nRoom < 0 ? 0 : nRoom];

		UnitRoomSceneUnitDesc desc{ kind.nUnitType, kind.nClassId, nRoom, room.nSubtileX + 3 + nOffset, room.nSubtileY + 5 + 2 * nOffset };
		desc.nClientY = 1000 + 7 * nOffset;
		desc.nAlignment = kind.nAlignment;
		desc.dwFlags = dwFlags;
		return desc;
	}

	void set_allies(UnitRoomScene& scene, int nAllies)
	{
		for (auto& room : scene.rooms)
		{
			room.tRoom.nAllies = nAllies;
		}
	}
}


TEST_SUITE("UnitRoomTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	TEST_CASE_FIXTURE(ItemStatCostTxtFixture<NoOpFixture>, "D2Common.0x6FDBCF10 (#11279)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_AddUnitToRoomEx, dll_base + 0x0007CF10);

		SUBCASE("")
		{
			for (auto kind_index = 0; kind_index < unit_kind_count; ++kind_index)
			{
				for (auto room_index = 0; room_index < 3; ++room_index)
				{
					// The room stored in the unit's path decides which update list is used
					for (const auto path_room : { room_index, -1, (room_index + 1) % 3 })
					{
						for (const uint32_t dwFlags : { 0u, (uint32_t)UNITFLAG_ISLINKREFRESHMSG })
						{
							// Input data
							std::vector<UnitRoomSceneUnitDesc> units = {
								make_unit(unit_kinds[0], room_index, 0),
								make_unit(unit_kinds[5], room_index, 1),
								make_unit(unit_kinds[6], room_index, 2),
								make_unit(unit_kinds[kind_index], room_index, 3, dwFlags),
							};
							units[3].nRoom = path_room;

							UnitRoomScene moo_scene(test_rooms, units);
							UnitRoomScene original_scene(test_rooms, units);
							int nUnused = room_index;

							const auto setup_data = [room_index](
								UnitRoomScene& scene
							) {
								scene.LinkUnitList(room_index, { 0, 1, 2 });
								scene.LinkUpdateList(0, { 1 });
								set_allies(scene, 2);
							};

							setup_data(moo_scene);
							setup_data(original_scene);

							// Call both implementations
							const auto moo_result = sut(moo_scene.Unit(3), moo_scene.Room(room_index), nUnused);
							const auto original_result = original(original_scene.Unit(3), original_scene.Room(room_index), nUnused);

							// Compare return values
							MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

							// Compare potentially modified input data
							check_scenes_eq(moo_scene, original_scene);
						}
					}
				}
			}
		}

		SUBCASE("Empty room")
		{
			for (auto kind_index = 0; kind_index < unit_kind_count; ++kind_index)
			{
				// Input data
				const std::vector<UnitRoomSceneUnitDesc> units = {
					make_unit(unit_kinds[kind_index], 1, 4),
				};

				UnitRoomScene moo_scene(test_rooms, units);
				UnitRoomScene original_scene(test_rooms, units);
				int nUnused = 0;

				// Call both implementations
				const auto moo_result = sut(moo_scene.Unit(0), moo_scene.Room(1), nUnused);
				const auto original_result = original(original_scene.Unit(0), original_scene.Room(1), nUnused);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				check_scenes_eq(moo_scene, original_scene);
			}
		}
	}

	TEST_CASE_FIXTURE(ItemStatCostTxtFixture<NoOpFixture>, "D2Common.0x6FDBD100 (#10384)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_AddUnitToRoom, dll_base + 0x0007D100);

		SUBCASE("")
		{
			for (auto kind_index = 0; kind_index < unit_kind_count; ++kind_index)
			{
				for (auto room_index = 0; room_index < 3; ++room_index)
				{
					// Input data
					const std::vector<UnitRoomSceneUnitDesc> units = {
						make_unit(unit_kinds[2], room_index, 0),
						make_unit(unit_kinds[1], room_index, 1),
						make_unit(unit_kinds[kind_index], room_index, 2),
					};

					UnitRoomScene moo_scene(test_rooms, units);
					UnitRoomScene original_scene(test_rooms, units);

					const auto setup_data = [room_index](
						UnitRoomScene& scene
					) {
						scene.LinkUnitList(room_index, { 1, 0 });
						scene.LinkUpdateList(room_index, { 0 });
						set_allies(scene, 1);
					};

					setup_data(moo_scene);
					setup_data(original_scene);

					// Call both implementations
					const auto moo_result = sut(moo_scene.Unit(2), moo_scene.Room(room_index));
					const auto original_result = original(original_scene.Unit(2), original_scene.Room(room_index));

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					check_scenes_eq(moo_scene, original_scene);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD120 (#10385)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_RefreshUnit, dll_base + 0x0007D120);

		SUBCASE("")
		{
			for (auto kind_index = 0; kind_index < unit_kind_count; ++kind_index)
			{
				for (const auto path_room : { -1, 0, 1, 2 })
				{
					for (const uint32_t dwFlags : { 0u, (uint32_t)UNITFLAG_ISLINKREFRESHMSG, (uint32_t)(UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET) })
					{
						for (const auto bHasPath : { true, false })
						{
							// Input data
							std::vector<UnitRoomSceneUnitDesc> units = {
								make_unit(unit_kinds[0], 0, 0),
								make_unit(unit_kinds[8], 1, 1),
								make_unit(unit_kinds[kind_index], path_room, 2, dwFlags),
							};

							const auto unit_type = unit_kinds[kind_index].nUnitType;
							if (!bHasPath)
							{
								// Only units with a dynamic path handle a missing path
								if (unit_type == UNIT_OBJECT || unit_type == UNIT_ITEM || unit_type == UNIT_TILE)
								{
									continue;
								}
								units[2].bHasPath = FALSE;
							}

							UnitRoomScene moo_scene(test_rooms, units);
							UnitRoomScene original_scene(test_rooms, units);

							const auto setup_data = [](
								UnitRoomScene& scene
							) {
								scene.LinkUnitList(0, { 0, 2 });
								scene.LinkUpdateList(0, { 0 });
								scene.LinkUpdateList(1, { 1 });
							};

							setup_data(moo_scene);
							setup_data(original_scene);

							// Call both implementations
							sut(moo_scene.Unit(2));
							original(original_scene.Unit(2));

							// Compare potentially modified input data
							check_scenes_eq(moo_scene, original_scene);
						}
					}
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD1B0 (#10388)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_SortUnitListByTargetY, dll_base + 0x0007D1B0);

		SUBCASE("Fixed orders")
		{
			const std::vector<std::vector<int>> client_ys = {
				{},
				{ 5 },
				{ 1, 2 },
				{ 2, 1 },
				{ 3, 3, 3 },
				{ 1, 2, 3, 4, 5, 6 },
				{ 6, 5, 4, 3, 2, 1 },
				{ 4, 1, 3, 1, 2, 4, 0 },
				{ -5, 10, -20, 0, 7, -1 },
			};

			for (const auto& ys : client_ys)
			{
				// Input data
				std::vector<UnitRoomSceneUnitDesc> units;
				std::vector<int> list;
				for (size_t i = 0; i < ys.size(); ++i)
				{
					auto desc = make_unit(unit_kinds[i % unit_kind_count], 0, static_cast<int>(i));
					desc.nClientY = ys[i];
					units.push_back(desc);
					list.push_back(static_cast<int>(i));
				}

				UnitRoomScene moo_scene(test_rooms, units);
				UnitRoomScene original_scene(test_rooms, units);

				const auto setup_data = [&list](
					UnitRoomScene& scene
				) {
					scene.LinkUnitList(0, list);
				};

				setup_data(moo_scene);
				setup_data(original_scene);

				// Call both implementations
				sut(moo_scene.Room(0));
				original(original_scene.Room(0));

				// Compare potentially modified input data
				check_scenes_eq(moo_scene, original_scene);
			}
		}

		SUBCASE("Random orders")
		{
			REPEAT_20();

			const auto unit_count = static_cast<int>(random_unsigned_integer(0, 12));
			const auto max_y = random_unsigned_integer(1, 2000);

			// Input data
			std::vector<UnitRoomSceneUnitDesc> units;
			std::vector<int> list;
			for (auto i = 0; i < unit_count; ++i)
			{
				auto desc = make_unit(unit_kinds[random_unsigned_integer(0, unit_kind_count - 1)], 0, i);
				desc.nClientY = static_cast<int>(random_unsigned_integer(0, max_y));
				units.push_back(desc);
				list.push_back(i);
			}

			UnitRoomScene moo_scene(test_rooms, units);
			UnitRoomScene original_scene(test_rooms, units);

			const auto setup_data = [&list](
				UnitRoomScene& scene
			) {
				scene.LinkUnitList(0, list);
			};

			setup_data(moo_scene);
			setup_data(original_scene);

			// Call both implementations
			sut(moo_scene.Room(0));
			original(original_scene.Room(0));

			// Compare potentially modified input data
			check_scenes_eq(moo_scene, original_scene);
		}
	}

	TEST_CASE_FIXTURE(ItemStatCostTxtFixture<NoOpFixture>, "D2Common.0x6FDBD250 (#10390)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_UpdatePath, dll_base + 0x0007D250);

		SUBCASE("")
		{
			for (auto kind_index = 0; kind_index < unit_kind_count; ++kind_index)
			{
				for (const auto path_room : { -1, 0, 1 })
				{
					for (const auto bInUnitList : { true, false })
					{
						for (const auto bInUpdateList : { true, false })
						{
							// Input data
							const std::vector<UnitRoomSceneUnitDesc> units = {
								make_unit(unit_kinds[5], 0, 0),
								make_unit(unit_kinds[kind_index], path_room, 1),
								make_unit(unit_kinds[9], 0, 2),
							};

							UnitRoomScene moo_scene(test_rooms, units);
							UnitRoomScene original_scene(test_rooms, units);

							const auto setup_data = [bInUnitList, bInUpdateList](
								UnitRoomScene& scene
							) {
								scene.LinkUnitList(0, bInUnitList ? std::vector<int>{ 0, 1, 2 } : std::vector<int>{ 0, 2 });
								scene.LinkUpdateList(0, bInUpdateList ? std::vector<int>{ 2, 1, 0 } : std::vector<int>{ 2, 0 });
								set_allies(scene, 3);
							};

							setup_data(moo_scene);
							setup_data(original_scene);

							// Call both implementations
							sut(moo_scene.Unit(1));
							original(original_scene.Unit(1));

							// Compare potentially modified input data
							check_scenes_eq(moo_scene, original_scene);
						}
					}
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD2B0 (#10391)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_ClearUpdateQueue, dll_base + 0x0007D2B0);

		SUBCASE("")
		{
			for (auto unit_count = 0; unit_count <= unit_kind_count; ++unit_count)
			{
				// Input data
				std::vector<UnitRoomSceneUnitDesc> units = {
					make_unit(unit_kinds[0], 1, 0),
					make_unit(unit_kinds[2], 1, 1, UNITFLAG_ISLINKREFRESHMSG),
				};
				std::vector<int> list;
				for (auto i = 0; i < unit_count; ++i)
				{
					units.push_back(make_unit(unit_kinds[i], 0, i, UNITFLAG_CANBEATTACKED));
					list.push_back(unit_count + 1 - i);
				}

				UnitRoomScene moo_scene(test_rooms, units);
				UnitRoomScene original_scene(test_rooms, units);

				const auto setup_data = [&list](
					UnitRoomScene& scene
				) {
					scene.LinkUpdateList(0, list);
					scene.LinkUpdateList(1, { 0 });
				};

				setup_data(moo_scene);
				setup_data(original_scene);

				// Call both implementations
				sut(moo_scene.Room(0));
				original(original_scene.Room(0));

				// Compare potentially modified input data
				check_scenes_eq(moo_scene, original_scene);
			}
		}
	}

	TEST_CASE_FIXTURE(ItemStatCostTxtFixture<NoOpFixture>, "D2Common.0x6FDBD300 (#10386)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_RemoveUnitFromRoom, dll_base + 0x0007D300);

		SUBCASE("")
		{
			for (auto kind_offset = 0; kind_offset < unit_kind_count; ++kind_offset)
			{
				// Units 0-4 are in the unit list of room 0, 5 is in room 0 but not in its list, 6 has no room and 7 has no path
				for (auto unit_index = 0; unit_index < 8; ++unit_index)
				{
					// Input data
					std::vector<UnitRoomSceneUnitDesc> units;
					for (auto i = 0; i < 7; ++i)
					{
						units.push_back(make_unit(unit_kinds[(kind_offset + i) % unit_kind_count], i < 6 ? 0 : -1, i, UNITFLAG_ISVALIDTARGET));
					}
					units.push_back(make_unit(unit_kinds[kind_offset % 2 ? 0 : 3], 0, 7));
					units.back().bHasPath = FALSE;

					UnitRoomScene moo_scene(test_rooms, units);
					UnitRoomScene original_scene(test_rooms, units);

					const auto setup_data = [](
						UnitRoomScene& scene
					) {
						scene.LinkUnitList(0, { 0, 1, 2, 3, 4 });
						scene.LinkUpdateList(0, { 4, 1, 5, 2 });
						scene.Unit(6)->dwFlags |= UNITFLAG_ISLINKREFRESHMSG;
						scene.Unit(7)->dwFlags |= UNITFLAG_ISLINKREFRESHMSG;
						set_allies(scene, 5);
					};

					setup_data(moo_scene);
					setup_data(original_scene);

					// Call both implementations
					sut(moo_scene.Unit(unit_index));
					original(original_scene.Unit(unit_index));

					// Compare potentially modified input data
					check_scenes_eq(moo_scene, original_scene);
				}
			}
		}

		SUBCASE("Unit is linked into the unit list of another room")
		{
			for (auto kind_index = 0; kind_index < unit_kind_count; ++kind_index)
			{
				for (const auto bRoomEmpty : { true, false })
				{
					// Input data
					const std::vector<UnitRoomSceneUnitDesc> units = {
						make_unit(unit_kinds[2], 0, 0),
						make_unit(unit_kinds[kind_index], 0, 1, UNITFLAG_ISLINKREFRESHMSG),
						make_unit(unit_kinds[9], 1, 2),
					};

					UnitRoomScene moo_scene(test_rooms, units);
					UnitRoomScene original_scene(test_rooms, units);

					const auto setup_data = [bRoomEmpty](
						UnitRoomScene& scene
					) {
						scene.LinkUnitList(0, bRoomEmpty ? std::vector<int>{} : std::vector<int>{ 0 });
						scene.LinkUnitList(1, { 1, 2 });
						set_allies(scene, 1);
					};

					setup_data(moo_scene);
					setup_data(original_scene);

					// Call both implementations
					sut(moo_scene.Unit(1));
					original(original_scene.Unit(1));

					// Compare potentially modified input data
					check_scenes_eq(moo_scene, original_scene);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD400 (#10387)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_RemoveUnitFromUpdateQueue, dll_base + 0x0007D400);

		SUBCASE("")
		{
			for (auto kind_offset = 0; kind_offset < unit_kind_count; ++kind_offset)
			{
				// Units 0-3 are in the update list of room 0, 4 is in room 0 but not in its update list, 5 has no room and 6 has no path
				for (auto unit_index = 0; unit_index < 7; ++unit_index)
				{
					// Input data
					std::vector<UnitRoomSceneUnitDesc> units;
					for (auto i = 0; i < 6; ++i)
					{
						units.push_back(make_unit(unit_kinds[(kind_offset + i) % unit_kind_count], i < 5 ? 0 : -1, i));
					}
					units.push_back(make_unit(unit_kinds[kind_offset % 2 ? 1 : 8], 0, 6));
					units.back().bHasPath = FALSE;

					UnitRoomScene moo_scene(test_rooms, units);
					UnitRoomScene original_scene(test_rooms, units);

					const auto setup_data = [](
						UnitRoomScene& scene
					) {
						scene.LinkUnitList(0, { 0, 1, 2, 3, 4 });
						scene.LinkUpdateList(0, { 3, 1, 0, 2 });
					};

					setup_data(moo_scene);
					setup_data(original_scene);

					// Call both implementations
					sut(moo_scene.Unit(unit_index));
					original(original_scene.Unit(unit_index));

					// Compare potentially modified input data
					check_scenes_eq(moo_scene, original_scene);
				}
			}
		}

		SUBCASE("Empty update list")
		{
			for (auto kind_index = 0; kind_index < unit_kind_count; ++kind_index)
			{
				// Input data
				const std::vector<UnitRoomSceneUnitDesc> units = {
					make_unit(unit_kinds[kind_index], 1, 0, UNITFLAG_ISLINKREFRESHMSG),
				};

				UnitRoomScene moo_scene(test_rooms, units);
				UnitRoomScene original_scene(test_rooms, units);

				// Call both implementations
				sut(moo_scene.Unit(0));
				original(original_scene.Unit(0));

				// Compare potentially modified input data
				check_scenes_eq(moo_scene, original_scene);
			}
		}

		SUBCASE("Unit is linked into the update list of another room")
		{
			for (auto kind_index = 0; kind_index < unit_kind_count; ++kind_index)
			{
				for (const auto bListEmpty : { true, false })
				{
					// Input data
					const std::vector<UnitRoomSceneUnitDesc> units = {
						make_unit(unit_kinds[3], 0, 0),
						make_unit(unit_kinds[kind_index], 0, 1),
						make_unit(unit_kinds[6], 1, 2),
					};

					UnitRoomScene moo_scene(test_rooms, units);
					UnitRoomScene original_scene(test_rooms, units);

					const auto setup_data = [bListEmpty](
						UnitRoomScene& scene
					) {
						scene.LinkUnitList(0, { 0, 1 });
						scene.LinkUpdateList(0, bListEmpty ? std::vector<int>{} : std::vector<int>{ 0 });
						scene.LinkUpdateList(1, { 1, 2 });
					};

					setup_data(moo_scene);
					setup_data(original_scene);

					// Call both implementations
					sut(moo_scene.Unit(1));
					original(original_scene.Unit(1));

					// Compare potentially modified input data
					check_scenes_eq(moo_scene, original_scene);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD4C0 (#10389)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_IsUnitInRoom, dll_base + 0x0007D4C0);

		SUBCASE("")
		{
			// Units 0-4 are in room 0, 5-6 are in room 1, 7 is in no list
			for (auto room_index = -1; room_index < 2; ++room_index)
			{
				for (auto unit_index = 0; unit_index < 8; ++unit_index)
				{
					// Input data
					std::vector<UnitRoomSceneUnitDesc> units;
					for (auto i = 0; i < 8; ++i)
					{
						units.push_back(make_unit(unit_kinds[i], i < 5 ? 0 : 1, i));
					}

					UnitRoomScene moo_scene(test_rooms, units);
					UnitRoomScene original_scene(test_rooms, units);

					const auto setup_data = [](
						UnitRoomScene& scene
					) {
						scene.LinkUnitList(0, { 4, 3, 0, 1, 2 });
						scene.LinkUnitList(1, { 6, 5 });
					};

					setup_data(moo_scene);
					setup_data(original_scene);

					// Call both implementations
					const auto moo_result = sut(moo_scene.Room(room_index), moo_scene.Unit(unit_index));
					const auto original_result = original(original_scene.Room(room_index), original_scene.Unit(unit_index));

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					check_scenes_eq(moo_scene, original_scene);

					// Check specific values
					CHECK_EQ(moo_result, (BOOL)((room_index == 0 && unit_index < 5) || (room_index == 1 && (unit_index == 5 || unit_index == 6))));
				}
			}
		}

		// NOTE: The original implementation dereferences the empty unit list (D2Common.0x6FDBD500) and crashes, so only D2MOO is checked
		SUBCASE("Empty room")
		{
			for (auto kind_index = 0; kind_index < unit_kind_count; ++kind_index)
			{
				// Input data
				const std::vector<UnitRoomSceneUnitDesc> units = {
					make_unit(unit_kinds[kind_index], 2, 0),
				};

				UnitRoomScene moo_scene(test_rooms, units);

				// Call D2MOO implementation
				const auto moo_result = sut(moo_scene.Room(2), moo_scene.Unit(0));

				// Check specific values
				CHECK_EQ(moo_result, FALSE);
			}
		}
	}
}
#endif
