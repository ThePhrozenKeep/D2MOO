#include <D2CommonTestDefines.h>

#ifdef MONSTERS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>
#include <set>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Monsters.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Units/Units.h>

#include <Fixtures/D2Lang/D2LangStringTblFixture.h>
#include <Fixtures/DataTbls/Fixtures.h>


TEST_SUITE("D2MonstersTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	TEST_CASE_FIXTURE(HirelingTxtFixture<ItemStatCostTxtFixture<NoOpFixture>>, "D2Common.0x6FDA4C10 (#11082)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_HirelingInit, dll_base + 0x00064C10);

		REPEAT_10();

		SUBCASE("")
		{
			const auto expansion = GENERATE(0, 1);
			const auto difficulty = GENERATE(0, 1, 2);

			for (auto act = 0; act < 5; ++act)
			{
				// Input data
				const auto level = random_unsigned_integer(1, 99);

				D2UnitStrc moo_pMonster{};
				D2StatListExStrc moo_pStatListEx{};
				D2StatStrc moo_pStat{};
				D2HirelingInitStrc moo_pHirelingInit{};
				D2UnitStrc original_pMonster{};
				D2StatListExStrc original_pStatListEx{};
				D2StatStrc original_pStat{};
				D2HirelingInitStrc original_pHirelingInit{};
				BOOL bExpansion = expansion;
				int nLowSeed = random_unsigned_integer();
				int nAct = act;
				int nDifficulty = difficulty;

				const auto setup_data = [level](
					D2UnitStrc& pMonster,
					D2StatListExStrc& pStatListEx,
					D2StatStrc& pStat,
					D2HirelingInitStrc& pHirelingInit
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.pStatListEx = &pStatListEx;
						pStatListEx.dwFlags |= STATLIST_EXTENDED;
						pStatListEx.FullStats.pStat = &pStat;
						pStatListEx.FullStats.nStatCount = 1;
						pStat.nStat = STAT_LEVEL;
						pStat.nValue = level;
					};

				setup_data(moo_pMonster, moo_pStatListEx, moo_pStat, moo_pHirelingInit);
				setup_data(original_pMonster, original_pStatListEx, original_pStat, original_pHirelingInit);

				// Call both implementations
				const auto moo_result = sut(bExpansion, &moo_pMonster, nLowSeed, nAct, nDifficulty, &moo_pHirelingInit);
				const auto original_result = original(bExpansion, &original_pMonster, nLowSeed, nAct, nDifficulty, &original_pHirelingInit);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
				MOO_CHECK_EQ(moo_pHirelingInit, original_pHirelingInit, "Comparing pHirelingInit");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA4E20 (#11081)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11081, dll_base + 0x00064E20);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			const auto hireling_init_id = GENERATE(0, 1, 2, 3);

			D2HirelingInitStrc moo_pHirelingInit{};
			D2HirelingInitStrc original_pHirelingInit{};
			int nLowSeed = random_unsigned_integer();;
			uint8_t a3 = GENERATE(0, 1, 2);

			const auto setup_data = [hireling_init_id](
				D2HirelingInitStrc& pHirelingInit
				) {
					pHirelingInit.nId = hireling_init_id;
				};

			setup_data(moo_pHirelingInit);
			setup_data(original_pHirelingInit);

			// Call both implementations
			const auto moo_result = sut(nLowSeed, &moo_pHirelingInit, a3);
			const auto original_result = original(nLowSeed, &original_pHirelingInit, a3);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pHirelingInit, original_pHirelingInit, "Comparing pHirelingInit");
		}
	}

	TEST_CASE_FIXTURE(D2LangStringTblFixture<NoOpFixture>, "D2Common.0x6FDA51A0 (#11085)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetHirelingDescString, dll_base + 0x000651A0);

		SUBCASE("")
		{
			constexpr short expected_calls[] = { 0, 0, 3371, 3372, 11090, 11088, 11089, 3377, 3376, 3375, 0 };
			for (auto i = -1; i < 11; ++i)
			{
				int nId = i;

				// Call both implementations
				d2lang_string_tbl_calls.clear();
				const auto moo_result = sut(nId);
				const auto moo_calls = d2lang_string_tbl_calls;

				d2lang_string_tbl_calls.clear();
				const auto original_result = original(nId);
				const auto original_calls = d2lang_string_tbl_calls;

				// Compare return values (the mocked string's first character is the requested tbl index)
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare requested tbl indices
				CHECK_EQ(moo_calls.size(), original_calls.size());
				CHECK_EQ(moo_calls, original_calls);

				if (!moo_calls.empty())
				{
					CHECK_EQ(moo_calls[0], expected_calls[i + 1]);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(HirelingTxtFixture<MonStatsTxtFixture<NoOpFixture>>, "D2Common.0x6FDA51C0 (#11086)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetActFromHirelingTxt, dll_base + 0x000651C0);

		SUBCASE("bExpansion = FALSE, valid nClassId")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				BOOL bExpansion = FALSE;
				int nClassId = i;
				uint16_t nNameId{};

				// Call both implementations
				const auto moo_result = sut(bExpansion, nClassId, nNameId);
				const auto original_result = original(bExpansion, nClassId, nNameId);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}

		SUBCASE("bExpansion = TRUE, valid nClassId")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				BOOL bExpansion = TRUE;
				int nClassId = i;
				uint16_t nNameId{};

				// Call both implementations
				const auto moo_result = sut(bExpansion, nClassId, nNameId);
				const auto original_result = original(bExpansion, nClassId, nNameId);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}

		SUBCASE("")
		{
			std::set<std::pair<uint16_t, uint16_t>> unique_name_ids;
			for (auto i = 0; i < hireling_record_count; ++i)
			{
				const auto& hireling_record = hireling_txt[i];
				unique_name_ids.insert({ hireling_record.wNameFirst, hireling_record.wNameLast });
			}

			SUBCASE("bExpansion = FALSE, invalid nClassId")
			{
				for (auto pair : unique_name_ids)
				{
					for (auto name_id = pair.first; name_id < pair.second; ++name_id)
					{
						BOOL bExpansion = FALSE;
						int nClassId = -1;
						uint16_t nNameId = name_id;

						// Call both implementations
						const auto moo_result = sut(bExpansion, nClassId, nNameId);
						const auto original_result = original(bExpansion, nClassId, nNameId);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}
			}

			SUBCASE("bExpansion = TRUE, valid nClassId")
			{
				for (auto pair : unique_name_ids)
				{
					for (auto name_id = pair.first; name_id < pair.second; ++name_id)
					{
						BOOL bExpansion = TRUE;
						int nClassId = -1;
						uint16_t nNameId = name_id;

						// Call both implementations
						const auto moo_result = sut(bExpansion, nClassId, nNameId);
						const auto original_result = original(bExpansion, nClassId, nNameId);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5200 (#11084)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetHirelingExpForNextLevel, dll_base + 0x00065200);

		SUBCASE("")
		{
			for (auto i = 1; i < 100; ++i)
			{
				int nLevel = i;
				int nExpPerLevel = random_unsigned_integer(0, 65535);

				// Call both implementations
				const auto moo_result = sut(nLevel, nExpPerLevel);
				const auto original_result = original(nLevel, nExpPerLevel);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(ItemStatCostTxtFixture<NoOpFixture>, "D2Common.0x6FDA5220 (#11083)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetHirelingResurrectionCost, dll_base + 0x00065220);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			const auto level = random_unsigned_integer(1, 100);

			D2UnitStrc moo_pHireling{};
			D2StatListExStrc moo_pStatListEx{};
			D2StatStrc moo_pStat{};
			D2UnitStrc original_pHireling{};
			D2StatListExStrc original_pStatListEx{};
			D2StatStrc original_pStat{};

			const auto setup_data = [level](
				D2UnitStrc& pHireling,
				D2StatListExStrc& pStatListEx,
				D2StatStrc& pStat
				) {
					pHireling.pStatListEx = &pStatListEx;
					pStatListEx.dwFlags |= STATLIST_EXTENDED;
					pStatListEx.FullStats.pStat = &pStat;
					pStatListEx.FullStats.nStatCount = 1;
					pStat.nStat = STAT_LEVEL;
					pStat.nValue = level;
				};

			setup_data(moo_pHireling, moo_pStatListEx, moo_pStat);
			setup_data(original_pHireling, original_pStatListEx, original_pStat);

			// Call both implementations
			const auto moo_result = sut(&moo_pHireling);
			const auto original_result = original(&original_pHireling);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pHireling, original_pHireling, "Comparing pHireling");
		}
	}

	TEST_CASE_FIXTURE(MonStats2TxtFixture<MonStatsTxtFixture<NoOpFixture>>, "D2Common.0x6FDA5270 (#11068)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11068_GetCompInfo, dll_base + 0x00065270);

		REPEAT_10();

		SUBCASE("")
		{
			for (auto i = 0; i < 16; ++i)
			{
				// Input data
				const auto class_id = random_unsigned_integer(0, monstats_record_count - 1);

				D2UnitStrc moo_pMonster{};
				D2UnitStrc original_pMonster{};
				int nComponent = i;

				const auto setup_data = [class_id](
					D2UnitStrc& pMonster
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = class_id;
					};

				setup_data(moo_pMonster);
				setup_data(original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonster, nComponent);
				const auto original_result = original(&original_pMonster, nComponent);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}
	}

	TEST_CASE_FIXTURE(LevelsTxtFixture<CompCodeTxtFixture<MonStats2TxtFixture<MonStatsTxtFixture<NoOpFixture>>>>, "D2Common.0x6FDA52F0 (#11069)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11069, dll_base + 0x000652F0);

		REPEAT_20();

		SUBCASE("")
		{
			const auto level_id = random_unsigned_integer(1, levels_record_count - 1);
			const auto class_id = random_unsigned_integer(0, monstats_record_count - 1);

			for (auto i = 0; i < 16; ++i)
			{
				for (auto j = 0; j < 12; ++j)
				{
					// Input data
					D2UnitStrc moo_pMonster{};
					D2UnitStrc original_pMonster{};
					D2DynamicPathStrc moo_pDynamicPath{};
					D2DynamicPathStrc original_pDynamicPath{};
					D2ActiveRoomStrc moo_pRoom{};
					D2ActiveRoomStrc original_pRoom{};
					D2DrlgRoomStrc moo_pDrlgRoom{};
					D2DrlgRoomStrc original_pDrlgRoom{};
					D2DrlgLevelStrc moo_pLevel{};
					D2DrlgLevelStrc original_pLevel{};
					unsigned int nIndex = i;
					unsigned int nComponent = j;

					const auto setup_data = [class_id, level_id](
						D2UnitStrc& pMonster,
						D2DynamicPathStrc& pDynamicPath,
						D2ActiveRoomStrc& pRoom,
						D2DrlgRoomStrc& pDrlgRoom,
						D2DrlgLevelStrc& pLevel
						) {
							pLevel.nLevelId = level_id;
							pDrlgRoom.pLevel = &pLevel;
							pRoom.pDrlgRoom = &pDrlgRoom;
							pDynamicPath.pRoom = &pRoom;
							pMonster.dwUnitType = UNIT_MONSTER;
							pMonster.dwClassId = class_id;
							pMonster.pDynamicPath = &pDynamicPath;
						};

					setup_data(moo_pMonster, moo_pDynamicPath, moo_pRoom, moo_pDrlgRoom, moo_pLevel);
					setup_data(original_pMonster, original_pDynamicPath, original_pRoom, original_pDrlgRoom, original_pLevel);

					// Call both implementations
					const auto moo_result = sut(&moo_pMonster, nIndex, nComponent);
					const auto original_result = original(&original_pMonster, nIndex, nComponent);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(CompCodeTxtFixture<MonStats2TxtFixture<MonStatsTxtFixture<NoOpFixture>>>, "D2Common.0x6FDA5450 (#11070)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11070, dll_base + 0x00065450);

		REPEAT_20();

		SUBCASE("")
		{
			const auto monster_id = random_unsigned_integer(0, monstats_record_count - 1);

			for (auto i = 0; i < 16; ++i)
			{
				for (auto j = 0; j < 12; ++j)
				{
					int nMonsterId = monster_id;
					unsigned int nComponent = i;
					unsigned int a3 = j;

					// Call both implementations
					const auto moo_result = sut(nMonsterId, nComponent, a3);
					const auto original_result = original(nMonsterId, nComponent, a3);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA54E0 (#11050)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11050, dll_base + 0x000654E0);

		SUBCASE("a2 = 0")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				for (auto j = 0; j < NUMBER_OF_MONMODES; ++j)
				{
					// Input data
					D2UnitStrc moo_pUnit{};
					D2UnitStrc original_pUnit{};
					int a2 = 0;

					const auto setup_data = [i, j](
						D2UnitStrc& pUnit
						) {
							pUnit.dwUnitType = UNIT_MONSTER;
							pUnit.dwClassId = i;
							pUnit.dwAnimMode = j;
						};

					setup_data(moo_pUnit);
					setup_data(original_pUnit);

					// Call both implementations
					const auto moo_result = sut(&moo_pUnit, a2);
					const auto original_result = original(&original_pUnit, a2);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
				}
			}
		}

		SUBCASE("a2 = 1")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				for (auto j = 0; j < NUMBER_OF_MONMODES; ++j)
				{
					// The sequence mode would require the MonSeq.txt to compute the event frames
					if (j == MONMODE_SEQUENCE)
					{
						continue;
					}

					// Input data
					const auto action_frame = random_unsigned_integer(0, 1);
					const auto frame = random_unsigned_integer(4, D2AnimDataRecordStrc::MAX_FRAME_FLAGS - 1);
					const auto anim_speed = random_unsigned_integer(0, 4 << 8);
					uint8_t frame_flags[D2AnimDataRecordStrc::MAX_FRAME_FLAGS]{};
					for (auto& frame_flag : frame_flags)
					{
						frame_flag = random_unsigned_integer(ANIMSEQ_EVENT_NONE, ANIMSEQ_EVENT_TRIGGER_SKILL);
					}

					D2UnitStrc moo_pUnit{};
					D2AnimDataRecordStrc moo_pAnimData{};
					D2UnitStrc original_pUnit{};
					D2AnimDataRecordStrc original_pAnimData{};
					int a2 = 1;

					const auto setup_data = [i, j, action_frame, frame, anim_speed, &frame_flags](
						D2UnitStrc& pUnit,
						D2AnimDataRecordStrc& pAnimData
						) {
							std::memcpy(pAnimData.pFrameFlags, frame_flags, sizeof(frame_flags));

							pUnit.dwUnitType = UNIT_MONSTER;
							pUnit.dwClassId = i;
							pUnit.dwAnimMode = j;
							pUnit.nActionFrame = action_frame;
							pUnit.nSeqCurrentFramePrecise = frame << 8;
							pUnit.wAnimSpeed = anim_speed;
							pUnit.pAnimData = &pAnimData;
						};

					setup_data(moo_pUnit, moo_pAnimData);
					setup_data(original_pUnit, original_pAnimData);

					// Call both implementations
					const auto moo_result = sut(&moo_pUnit, a2);
					const auto original_result = original(&original_pUnit, a2);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA55E0 (#11052)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11052, dll_base + 0x000655E0);

		SUBCASE("")
		{
			for (auto i = 0; i < 64; ++i)
			{
				uint8_t a1 = i;

				// Call both implementations
				const auto moo_result = sut(a1);
				const auto original_result = original(a1);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5600 (#11053)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11053, dll_base + 0x00065600);

		SUBCASE("")
		{
			for (auto i = 0; i < 64; ++i)
			{
				uint8_t a1 = i;

				// Call both implementations
				const auto moo_result = sut(a1);
				const auto original_result = original(a1);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5620 (#11054)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11054, dll_base + 0x00065620);

		SUBCASE("")
		{
			for (auto i = 0; i < 8; ++i)
			{
				uint8_t a1 = i;

				// Call both implementations
				const auto moo_result = sut(a1);
				const auto original_result = original(a1);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5640 (#11055)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11055, dll_base + 0x00065640);

		SUBCASE("")
		{
			for (auto i = 0; i < 32; ++i)
			{
				// Input data
				int moo_a2{};
				int moo_a3{};
				int original_a2{};
				int original_a3{};
				uint8_t a1 = i;

				// Call both implementations
				sut(a1, &moo_a2, &moo_a3);
				original(a1, &original_a2, &original_a3);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_a2, original_a2, "Comparing a2");
				MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5670 (#11297)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_SetMonsterNameInMonsterData, dll_base + 0x00065670);

		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2MonsterDataStrc moo_pMonsterData{};
			D2UnitStrc original_pMonster{};
			D2MonsterDataStrc original_pMonsterData{};
			Unicode wszName[32]{ L'N', L'a', L'm', L'e' };

			const auto setup_data = [](
				D2UnitStrc& pMonster,
				D2MonsterDataStrc& pMonsterData
				) {
					pMonster.dwUnitType = UNIT_MONSTER;
					pMonster.pMonsterData = &pMonsterData;
				};

			setup_data(moo_pMonster, moo_pMonsterData);
			setup_data(original_pMonster, original_pMonsterData);

			// Call both implementations
			sut(&moo_pMonster, wszName);
			original(&original_pMonster, wszName);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA56C0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_CanBeInTown, dll_base + 0x000656C0);

		SUBCASE("")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				D2UnitStrc moo_pMonster{};
				D2UnitStrc original_pMonster{};

				const auto setup_data = [i](
					D2UnitStrc& pMonster
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
					};

				setup_data(moo_pMonster);
				setup_data(original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonster);
				const auto original_result = original(&original_pMonster);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA5750 (#11057)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsSandLeaper, dll_base + 0x00065750);

		SUBCASE("bAlwaysReturnFalse = FALSE")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				D2UnitStrc moo_pMonster{};
				D2UnitStrc original_pMonster{};
				BOOL bAlwaysReturnFalse = FALSE;

				const auto setup_data = [i](
					D2UnitStrc& pMonster
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
					};

				setup_data(moo_pMonster);
				setup_data(original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonster, bAlwaysReturnFalse);
				const auto original_result = original(&original_pMonster, bAlwaysReturnFalse);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}

		SUBCASE("bAlwaysReturnFalse = TRUE")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				D2UnitStrc moo_pMonster{};
				D2UnitStrc original_pMonster{};
				BOOL bAlwaysReturnFalse = TRUE;

				const auto setup_data = [i](
					D2UnitStrc& pMonster
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
					};

				setup_data(moo_pMonster);
				setup_data(original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonster, bAlwaysReturnFalse);
				const auto original_result = original(&original_pMonster, bAlwaysReturnFalse);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA57D0 (#11058)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsDemon, dll_base + 0x000657D0);

		SUBCASE("")
		{
			for (auto i = -1; i < monstats_record_count + 1; ++i)
			{
				// Input data
				D2UnitStrc moo_pMonster{};
				D2UnitStrc original_pMonster{};

				const auto setup_data = [i](
					D2UnitStrc& pMonster
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
					};

				setup_data(moo_pMonster);
				setup_data(original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonster);
				const auto original_result = original(&original_pMonster);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA5830 (#11059)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsUndead, dll_base + 0x00065830);

		SUBCASE("")
		{
			for (auto i = -1; i < monstats_record_count + 1; ++i)
			{
				// Input data
				D2UnitStrc moo_pMonster{};
				D2UnitStrc original_pMonster{};

				const auto setup_data = [i](
					D2UnitStrc& pMonster
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
					};

				setup_data(moo_pMonster);
				setup_data(original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonster);
				const auto original_result = original(&original_pMonster);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA58A0 (#11060)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsBoss, dll_base + 0x000658A0);

		SUBCASE("pMonStatsTxtRecord = nullptr")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				D2MonStatsTxt moo_pMonStatsTxtRecord{};
				D2UnitStrc moo_pMonster{};
				D2MonStatsTxt original_pMonStatsTxtRecord{};
				D2UnitStrc original_pMonster{};

				const auto setup_data = [i](
					D2MonStatsTxt& pMonStatsTxtRecord,
					D2UnitStrc& pMonster
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
					};

				setup_data(moo_pMonStatsTxtRecord, moo_pMonster);
				setup_data(original_pMonStatsTxtRecord, original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonStatsTxtRecord, &moo_pMonster);
				const auto original_result = original(&original_pMonStatsTxtRecord, &original_pMonster);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonStatsTxtRecord, original_pMonStatsTxtRecord, "Comparing pMonStatsTxtRecord");
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}

		SUBCASE("pMonStatsTxtRecord set")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				D2MonStatsTxt moo_pMonStatsTxtRecord{};
				D2UnitStrc moo_pMonster{};
				D2MonStatsTxt original_pMonStatsTxtRecord{};
				D2UnitStrc original_pMonster{};

				const auto setup_data = [this, i](
					D2MonStatsTxt& pMonStatsTxtRecord,
					D2UnitStrc& pMonster
					) {
						pMonStatsTxtRecord = monstats_txt[i];

						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
					};

				setup_data(moo_pMonStatsTxtRecord, moo_pMonster);
				setup_data(original_pMonStatsTxtRecord, original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonStatsTxtRecord, &moo_pMonster);
				const auto original_result = original(&original_pMonStatsTxtRecord, &original_pMonster);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonStatsTxtRecord, original_pMonStatsTxtRecord, "Comparing pMonStatsTxtRecord");
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5900 (#11064)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsDead, dll_base + 0x00065900);

		SUBCASE("")
		{
			for (auto i = 0; i < NUMBER_OF_MONMODES; ++i)
			{
				// Input data
				D2UnitStrc moo_pMonster{};
				D2UnitStrc original_pMonster{};

				const auto setup_data = [i](
					D2UnitStrc& pMonster
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwAnimMode = i;
					};

				setup_data(moo_pMonster);
				setup_data(original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonster);
				const auto original_result = original(&original_pMonster);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}
	}

	TEST_CASE_FIXTURE(SkillsTxtFixture<MonStatsTxtFixture<NoOpFixture>>, "D2Common.0x6FDA5930 (#11280)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetSpawnMode_XY, dll_base + 0x00065930);

		SUBCASE("")
		{
			const auto from_monster = GENERATE(0, 1);

			for (auto j = 0; j < skills_record_count; ++j)
			{
				for (auto i = 0; i < monstats_record_count; ++i)
				{
					// Input data
					const auto x = random_unsigned_integer(0, 65535);
					const auto y = random_unsigned_integer(0, 65535);

					D2UnitStrc moo_pMonster{};
					D2DynamicPathStrc moo_pDynamicPath{};
					int moo_pSpawnMode{};
					int moo_pX{};
					int moo_pY{};
					D2UnitStrc original_pMonster{};
					D2DynamicPathStrc original_pDynamicPath{};
					int original_pSpawnMode{};
					int original_pX{};
					int original_pY{};
					BOOL bFromMonster = from_monster;
					int nSkillId = j;
					int nSkillLevel = 0;

					const auto setup_data = [i, x, y](
						D2UnitStrc& pMonster,
						D2DynamicPathStrc& pDynamicPath,
						int& pSpawnMode,
						int& pX,
						int& pY
						) {
							pMonster.dwUnitType = UNIT_MONSTER;
							pMonster.dwClassId = i;
							pMonster.pDynamicPath = &pDynamicPath;
							pDynamicPath.tGameCoords.wPosX = x;
							pDynamicPath.tGameCoords.wPosY = y;
						};

					setup_data(moo_pMonster, moo_pDynamicPath, moo_pSpawnMode, moo_pX, moo_pY);
					setup_data(original_pMonster, original_pDynamicPath, original_pSpawnMode, original_pX, original_pY);

					// Call both implementations
					const auto moo_result = sut(&moo_pMonster, bFromMonster, nSkillId, nSkillLevel, &moo_pSpawnMode, &moo_pX, &moo_pY);
					const auto original_result = original(&original_pMonster, bFromMonster, nSkillId, nSkillLevel, &original_pSpawnMode, &original_pX, &original_pY);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
					MOO_CHECK_EQ(moo_pSpawnMode, original_pSpawnMode, "Comparing pSpawnMode");
					MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
					MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(LevelsTxtFixture<MonStatsTxtFixture<NoOpFixture>>, "D2Common.0x6FDA5B30 (#11061)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetMinionSpawnInfo, dll_base + 0x00065B30);

		SUBCASE("")
		{
			const auto difficulty = GENERATE(0, 1, 2);

			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				const auto x = random_unsigned_integer(0, 65535);
				const auto y = random_unsigned_integer(0, 65535);
				const auto target_x = random_unsigned_integer(0, 65535);
				const auto target_y = random_unsigned_integer(0, 65535);
				const auto level_id = random_unsigned_integer(1, levels_record_count - 1);
				const auto low_seed = random_unsigned_integer();
				const auto high_seed = random_unsigned_integer();

				D2UnitStrc moo_pMonster{};
				D2ActiveRoomStrc moo_pRoom{};
				D2DrlgRoomStrc moo_pDrlgRoom{};
				D2DrlgLevelStrc moo_pLevel{};
				D2SeedStrc moo_pSeed{};
				D2DynamicPathStrc moo_pDynamicPath{};
				int moo_pId{};
				int moo_pX{};
				int moo_pY{};
				int moo_pSpawnMode{};
				D2UnitStrc original_pMonster{};
				D2ActiveRoomStrc original_pRoom{};
				D2DrlgRoomStrc original_pDrlgRoom{};
				D2DrlgLevelStrc original_pLevel{};
				D2SeedStrc original_pSeed{};
				D2DynamicPathStrc original_pDynamicPath{};
				int original_pId{};
				int original_pX{};
				int original_pY{};
				int original_pSpawnMode{};
				int nDifficulty = difficulty;

				const auto setup_data = [i, x, y, target_x, target_y, level_id, low_seed, high_seed](
					D2UnitStrc& pMonster,
					D2ActiveRoomStrc& pRoom,
					D2DrlgRoomStrc& pDrlgRoom,
					D2DrlgLevelStrc& pLevel,
					D2SeedStrc& pSeed,
					D2DynamicPathStrc& pDynamicPath,
					int& pId,
					int& pX,
					int& pY,
					int& pSpawnMode
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
						pMonster.pSeed = pSeed;
						pMonster.pDynamicPath = &pDynamicPath;
						pDynamicPath.pRoom = &pRoom;
						pRoom.pDrlgRoom = &pDrlgRoom;
						pDrlgRoom.pLevel = &pLevel;
						pLevel.nLevelId = level_id;
						pDynamicPath.tGameCoords.wPosX = x;
						pDynamicPath.tGameCoords.wPosY = y;
						pDynamicPath.tTargetCoord.X = target_x;
						pDynamicPath.tTargetCoord.Y = target_y;
						pSeed.nLowSeed = low_seed;
						pSeed.nHighSeed = high_seed;
					};

				setup_data(moo_pMonster, moo_pRoom, moo_pDrlgRoom, moo_pLevel, moo_pSeed, moo_pDynamicPath, moo_pId, moo_pX, moo_pY, moo_pSpawnMode);
				setup_data(original_pMonster, original_pRoom, original_pDrlgRoom, original_pLevel, original_pSeed, original_pDynamicPath, original_pId, original_pX, original_pY, original_pSpawnMode);

				auto get_class_id = [](D2UnitStrc* monster) {
					return 123;
					};

				// Call both implementations
				sut(&moo_pMonster, &moo_pId, &moo_pX, &moo_pY, &moo_pSpawnMode, nDifficulty, get_class_id);
				original(&original_pMonster, &original_pId, &original_pX, &original_pY, &original_pSpawnMode, nDifficulty, get_class_id);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
				MOO_CHECK_EQ(moo_pId, original_pId, "Comparing pId");
				MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
				MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
				MOO_CHECK_EQ(moo_pSpawnMode, original_pSpawnMode, "Comparing pSpawnMode");
			}
		}
	}

	TEST_CASE_FIXTURE(ItemsTxtFixture<CompCodeTxtFixture<MonStats2TxtFixture<MonStatsTxtFixture<NoOpFixture>>>>, "D2Common.0x6FDA6410 (#11051)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetMaximalLightRadius, dll_base + 0x00066410);

		// Looking up items by their code requires the items linker, which is not set up by the ItemsTxtFixture
		const auto items_linker = static_cast<D2TxtLinkStrc*>(FOG_AllocLinker(__FILE__, __LINE__));
		for (auto i = 0; i < items_record_count; ++i)
		{
			FOG_10215(items_linker, items_txt[i].dwCode);
		}

		sgptDataTables->pItemsLinker = items_linker;

		const auto original_items_linker = reinterpret_cast<D2TxtLinkStrc**>(dll_base + 0x000A9608 + 0x00000094);
		*original_items_linker = items_linker;

		REPEAT_10();

		SUBCASE("")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				const auto monstats2_id = monstats_txt[i].wMonStatsEx;

				uint8_t components[16]{};
				for (auto j = 0; j < 16; ++j)
				{
					const auto choice_count = monstats2_id < monstats2_record_count ? monstats2_txt[monstats2_id].nComponentChoiceCounts[j] : 0;
					components[j] = choice_count > 0 ? random_unsigned_integer(0, choice_count - 1) : 0;
				}

				D2UnitStrc moo_pMonster{};
				D2MonsterDataStrc moo_pMonsterData{};
				D2UnitStrc original_pMonster{};
				D2MonsterDataStrc original_pMonsterData{};

				const auto setup_data = [i, &components](
					D2UnitStrc& pMonster,
					D2MonsterDataStrc& pMonsterData
					) {
						std::memcpy(pMonsterData.nComponent, components, sizeof(components));

						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
						pMonster.pMonsterData = &pMonsterData;
					};

				setup_data(moo_pMonster, moo_pMonsterData);
				setup_data(original_pMonster, original_pMonsterData);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonster);
				const auto original_result = original(&original_pMonster);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}

		*original_items_linker = nullptr;
		sgptDataTables->pItemsLinker = nullptr;
		FOG_FreeLinker(items_linker);
	}

	TEST_CASE_FIXTURE(LevelsTxtFixture<MonStatsTxtFixture<NoOpFixture>>, "D2Common.0x6FDA64B0 (#11063)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11063, dll_base + 0x000664B0);

		SUBCASE("")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				for (auto j = 1; j < levels_record_count; ++j)
				{
					// Input data
					D2ActiveRoomStrc moo_pRoom{};
					D2ActiveRoomStrc original_pRoom{};
					D2DrlgRoomStrc moo_pDrlgRoom{};
					D2DrlgRoomStrc original_pDrlgRoom{};
					D2DrlgLevelStrc moo_pLevel{};
					D2DrlgLevelStrc original_pLevel{};
					int nMonsterId = i;

					const auto setup_data = [j](
						D2ActiveRoomStrc& pRoom,
						D2DrlgRoomStrc& pDrlgRoom,
						D2DrlgLevelStrc& pLevel
						) {
							pLevel.nLevelId = j;
							pDrlgRoom.pLevel = &pLevel;
							pRoom.pDrlgRoom = &pDrlgRoom;
						};

					setup_data(moo_pRoom, moo_pDrlgRoom, moo_pLevel);
					setup_data(original_pRoom, original_pDrlgRoom, original_pLevel);

					// Call both implementations
					const auto moo_result = sut(&moo_pRoom, nMonsterId);
					const auto original_result = original(&original_pRoom, nMonsterId);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA6620 (#11065)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsPrimeEvil, dll_base + 0x00066620);

		SUBCASE("")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				D2UnitStrc moo_pMonster{};
				D2UnitStrc original_pMonster{};

				const auto setup_data = [i](
					D2UnitStrc& pMonster
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
					};

				setup_data(moo_pMonster);
				setup_data(original_pMonster);

				// Call both implementations
				const auto moo_result = sut(&moo_pMonster);
				const auto original_result = original(&original_pMonster);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA6680 (#11066)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11066, dll_base + 0x00066680);

		SUBCASE("")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				D2UnitStrc moo_pMonster{};
				int moo_pDirectionX{};
				int moo_pDirectionY{};
				D2UnitStrc original_pMonster{};
				int original_pDirectionX{};
				int original_pDirectionY{};

				const auto setup_data = [i](
					D2UnitStrc& pMonster,
					int& pDirectionX,
					int& pDirectionY
					) {
						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
					};

				setup_data(moo_pMonster, moo_pDirectionX, moo_pDirectionY);
				setup_data(original_pMonster, original_pDirectionX, original_pDirectionY);

				// Call both implementations
				sut(&moo_pMonster, &moo_pDirectionX, &moo_pDirectionY);
				original(&original_pMonster, &original_pDirectionX, &original_pDirectionY);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
				MOO_CHECK_EQ(moo_pDirectionX, original_pDirectionX, "Comparing pDirectionX");
				MOO_CHECK_EQ(moo_pDirectionY, original_pDirectionY, "Comparing pDirectionY");
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA6730 (#11067)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetHirelingTypeId, dll_base + 0x00066730);

		SUBCASE("")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				D2UnitStrc moo_pHireling{};
				D2UnitStrc original_pHireling{};

				const auto setup_data = [i](
					D2UnitStrc& pHireling
					) {
						pHireling.dwUnitType = UNIT_MONSTER;
						pHireling.dwClassId = i;
					};

				setup_data(moo_pHireling);
				setup_data(original_pHireling);

				// Call both implementations
				const auto moo_result = sut(&moo_pHireling);
				const auto original_result = original(&original_pHireling);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pHireling, original_pHireling, "Comparing pHireling");
			}
		}
	}

	TEST_CASE_FIXTURE(ItemStatCostTxtFixture<MonStatsTxtFixture<NoOpFixture>>, "D2Common.0x6FDA6790 (#11246)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_ApplyClassicScaling, dll_base + 0x00066790);

		SUBCASE("")
		{
			const auto expansion = GENERATE(FALSE, TRUE);
			const auto difficulty = GENERATE(0, 1, 2);

			for (auto i = 0; i < monstats_record_count; ++i)
			{
				// Input data
				// Stats are sorted by stat id. The capacity leaves room for inserting stats and is small enough to never
				// trigger a shrinking reallocation, as the arrays are not allocated through the memory pool.
				// The values are large enough to not be scaled down to 0, which would remove the stat from the arrays.
				constexpr auto stat_capacity = 8;
				D2StatStrc stats[stat_capacity]{};
				stats[0].nStat = STAT_MAXHP;
				stats[0].nValue = random_unsigned_integer(100, 65535) << 8;
				stats[1].nStat = STAT_LEVEL;
				stats[1].nValue = random_unsigned_integer(1, 99);
				stats[2].nStat = STAT_EXPERIENCE;
				stats[2].nValue = random_unsigned_integer(100, 65535);
				stats[3].nStat = STAT_ARMORCLASS;
				stats[3].nValue = random_unsigned_integer(100, 65535);
				const auto stat_count = 4;

				D2UnitStrc moo_pMonster{};
				D2StatListExStrc moo_pStatListEx{};
				D2StatStrc moo_pBaseStats[stat_capacity]{};
				D2StatStrc moo_pFullStats[stat_capacity]{};
				D2UnitStrc original_pMonster{};
				D2StatListExStrc original_pStatListEx{};
				D2StatStrc original_pBaseStats[stat_capacity]{};
				D2StatStrc original_pFullStats[stat_capacity]{};
				BOOL bExpansion = expansion;
				uint8_t nDifficulty = difficulty;

				const auto setup_data = [i, &stats, stat_count, stat_capacity](
					D2UnitStrc& pMonster,
					D2StatListExStrc& pStatListEx,
					D2StatStrc(&pBaseStats)[stat_capacity],
					D2StatStrc(&pFullStats)[stat_capacity]
					) {
						std::memcpy(pBaseStats, stats, sizeof(stats));
						std::memcpy(pFullStats, stats, sizeof(stats));

						pStatListEx.dwFlags |= STATLIST_EXTENDED;
						pStatListEx.Stats.pStat = pBaseStats;
						pStatListEx.Stats.nStatCount = stat_count;
						pStatListEx.Stats.nCapacity = stat_capacity;
						pStatListEx.FullStats.pStat = pFullStats;
						pStatListEx.FullStats.nStatCount = stat_count;
						pStatListEx.FullStats.nCapacity = stat_capacity;

						pMonster.dwUnitType = UNIT_MONSTER;
						pMonster.dwClassId = i;
						pMonster.pStatListEx = &pStatListEx;
					};

				setup_data(moo_pMonster, moo_pStatListEx, moo_pBaseStats, moo_pFullStats);
				setup_data(original_pMonster, original_pStatListEx, original_pBaseStats, original_pFullStats);

				// Call both implementations
				sut(&moo_pMonster, bExpansion, nDifficulty);
				original(&original_pMonster, bExpansion, nDifficulty);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA6920")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetBaseIdFromMonsterId, dll_base + 0x00066920);

		SUBCASE("")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				int nMonsterId = i;

				// Call both implementations
				const auto moo_result = sut(nMonsterId);
				const auto original_result = original(nMonsterId);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA6950")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetClassIdFromMonsterChain, dll_base + 0x00066950);

		SUBCASE("")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				for (auto j = 0; j < 16; ++j)
				{
					int nMonsterId = i;
					int nChainId = j;

					// Call both implementations
					const auto moo_result = sut(nMonsterId, nChainId);
					const auto original_result = original(nMonsterId, nChainId);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<NoOpFixture>, "D2Common.0x6FDA69C0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_ValidateMonsterId, dll_base + 0x000669C0);

		SUBCASE("")
		{
			for (auto i = 0; i < monstats_record_count; ++i)
			{
				int nMonsterId = i;

				// Call both implementations
				const auto moo_result = sut(nMonsterId);
				const auto original_result = original(nMonsterId);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}
}

#endif
