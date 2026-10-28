#include <D2CommonTestDefines.h>

#ifdef STATES_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Skills.h>
#include <D2States.h>
#include <Units/Units.h>

#include <Fixtures/DataTbls/Fixtures.h>
#include <Fixtures/UnitWithStatFlagsFixture.h>


DYNAMIC_ARRAY_TYPE(uint32_t)


TEST_SUITE("D2StatesTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4560 (#10486)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_ToggleState, dll_base + 0x00074560);

		REPEAT_10();

		const auto bSet = GENERATE(0, 1);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				const auto unit_flags_ex = random_unsigned_integer();

				int nState = i;

				const auto setup_data = [unit_flags_ex](
					D2UnitStrc& pUnit
				) {
					pUnit.dwFlagEx = unit_flags_ex;
				};

				setup_data(moo_pUnit);
				setup_data(original_pUnit);

				reroll_flags();

				// Call both implementations
				sut(&moo_pUnit, nState, bSet);
				original(&original_pUnit, nState, bSet);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
				auto wrapped_moo_StatFlags = DynamicArray<uint32_t>{ moo_StatFlags.get(), 2 * flag_count };
				auto wrapped_original_StatFlags = DynamicArray<uint32_t>{ original_StatFlags.get(), 2 * flag_count };
				MOO_CHECK_EQ(wrapped_moo_StatFlags, wrapped_original_StatFlags, "Comparing StatFlags");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB45A0 (#10487)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckState, dll_base + 0x000745A0);

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_MISSILE);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;

				const auto setup_data = [unit_type](
					D2UnitStrc& pUnit
				) {
					pUnit.dwUnitType = unit_type;
				};

				setup_data(moo_pUnit);
				setup_data(original_pUnit);

				reroll_flags();

				// Call both implementations
				const auto moo_result = sut(&moo_pUnit, nState);
				const auto original_result = original(&original_pUnit, nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB45F0 (#10488)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_ToggleGfxStateFlag, dll_base + 0x000745F0);

		const auto bSet = GENERATE(0, 1);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;

				reroll_flags();

				// Call both implementations
				sut(&moo_pUnit, nState, bSet);
				original(&original_pUnit, nState, bSet);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
				auto wrapped_moo_StatFlags = DynamicArray<uint32_t>{ moo_StatFlags.get(), 2 * flag_count };
				auto wrapped_original_StatFlags = DynamicArray<uint32_t>{ original_StatFlags.get(), 2 * flag_count };
				MOO_CHECK_EQ(wrapped_moo_StatFlags, wrapped_original_StatFlags, "Comparing StatFlags");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4670 (#10489)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckGfxStateFlag, dll_base + 0x00074670);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;
				// Call both implementations
				const auto moo_result = sut(&moo_pUnit, nState);
				const auto original_result = original(&original_pUnit, nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB46C0 (#10491)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_ClearGfxStateFlags, dll_base + 0x000746C0);

		SUBCASE("")
		{
			// Call both implementations
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			auto wrapped_moo_StatFlags = DynamicArray<uint32_t>{ moo_StatFlags.get(), 2 * flag_count };
			auto wrapped_original_StatFlags = DynamicArray<uint32_t>{ original_StatFlags.get(), 2 * flag_count };
			MOO_CHECK_EQ(wrapped_moo_StatFlags, wrapped_original_StatFlags, "Comparing StatFlags");

			// Check specific values
			for (auto i = 0; i < flag_count; ++i)
			{
				CHECK_EQ(moo_StatFlags[i + flag_count], 0);
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4710 (#10490)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_IsAnyGfxStateFlagSet, dll_base + 0x00074710);

		SUBCASE("none set")
		{
			const auto setup_data = [this](
				std::unique_ptr<uint32_t[]>& StatFlags
			) {
				std::memset(StatFlags.get(), 0, sizeof(uint32_t) * 2 * flag_count);
			};

			setup_data(moo_StatFlags);
			setup_data(original_StatFlags);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");

			CHECK_EQ(moo_result, 0);
		}

		SUBCASE("some set")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");

			CHECK_EQ(moo_result, 1);
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4760 (#10492)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_GetGfxStateFlags, dll_base + 0x00074760);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			auto wrapped_moo_result = DynamicArray<uint32_t>{ moo_result, flag_count };
			auto wrapped_original_result = DynamicArray<uint32_t>{ original_result, flag_count };
			MOO_CHECK_EQ(wrapped_moo_result, wrapped_original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4790 (#10493)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_UpdateStayDeathFlags, dll_base + 0x00074790);

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_MISSILE);
		const auto bIsBoss = GENERATE(0, 1);

		SUBCASE("")
		{
			const auto setup_data = [unit_type](
				D2UnitStrc& pUnit
			) {
				pUnit.dwUnitType = unit_type;
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, bIsBoss);
			original(&original_pUnit, bIsBoss);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			auto wrapped_moo_StatFlags = DynamicArray<uint32_t>{ moo_StatFlags.get(), 2 * flag_count };
			auto wrapped_original_StatFlags = DynamicArray<uint32_t>{ original_StatFlags.get(), 2 * flag_count };
			MOO_CHECK_EQ(wrapped_moo_StatFlags, wrapped_original_StatFlags, "Comparing StatFlags");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB48F0 (#10494)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10494_STATES_GetStatFlags, dll_base + 0x000748F0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			auto wrapped_moo_result = DynamicArray<uint32_t>{ moo_result, 2 * flag_count };
			auto wrapped_original_result = DynamicArray<uint32_t>{ original_result, 2 * flag_count };
			MOO_CHECK_EQ(wrapped_moo_result, wrapped_original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<NoOpFixture>, "D2Common.0x6FDB4900 (#10495)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_GetNumberOfStateFlags, dll_base + 0x00074900);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<NoOpFixture>, "D2Common.0x6FDB4920 (#10496)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskAuraByStateId, dll_base + 0x00074920);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;

				// Call both implementations
				const auto moo_result = sut(nState);
				const auto original_result = original(nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4960 (#10503)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskAuraOnUnit, dll_base + 0x00074960);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<NoOpFixture>, "D2Common.0x6FDB49D0 (#10500)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskPgsvByStateId, dll_base + 0x000749D0);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;

				// Call both implementations
				const auto moo_result = sut(nState);
				const auto original_result = original(nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4A10 (#11234)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskPgsvOnUnit, dll_base + 0x00074A10);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4A80 (#10501)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_UpdatePgsvFlags, dll_base + 0x00074A80);

		SUBCASE("")
		{
			// Call both implementations
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			auto wrapped_moo_StatFlags = DynamicArray<uint32_t>{ moo_StatFlags.get(), 2 * flag_count };
			auto wrapped_original_StatFlags = DynamicArray<uint32_t>{ original_StatFlags.get(), 2 * flag_count };
			MOO_CHECK_EQ(wrapped_moo_StatFlags, wrapped_original_StatFlags, "Comparing StatFlags");
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<NoOpFixture>, "D2Common.0x6FDB4B20 (#10558)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskHideByStateId, dll_base + 0x00074B20);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;

				// Call both implementations
				const auto moo_result = sut(nState);
				const auto original_result = original(nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4B50 (#10557)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskHideOnUnit, dll_base + 0x00074B50);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4BC0 (#10502)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskActiveOnUnit, dll_base + 0x00074BC0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4C30 (#11272)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRemHitOnUnit, dll_base + 0x00074C30);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4CA0 (#10538)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskDamBlueOnUnit, dll_base + 0x00074CA0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4D10 (#10545)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskDamRedOnUnit, dll_base + 0x00074D10);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4D80 (#10537)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskAttBlueOnUnit, dll_base + 0x00074D80);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4DF0 (#10544)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskAttRedOnUnit, dll_base + 0x00074DF0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB4E60 (#10551)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_IsUnitShapeShifted, dll_base + 0x00074E60);

		const auto is_set = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			const auto flags = random_unsigned_integer();

			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [is_set, flags](
				D2UnitStrc& pUnit
			) {
				if (is_set)
				{
					pUnit.dwFlagEx |= UNITFLAGEX_ISSHAPESHIFTED;
				}
				else
				{
					pUnit.dwFlagEx = flags & ~UNITFLAGEX_ISSHAPESHIFTED;
				}
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");

			// Check specific values
			CHECK_EQ(moo_result, is_set);
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<NoOpFixture>, "D2Common.0x6FDB4E80 (#10497)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskCurseByStateId, dll_base + 0x00074E80);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;

				// Call both implementations
				const auto moo_result = sut(nState);
				const auto original_result = original(nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<NoOpFixture>, "D2Common.0x6FDB4EC0 (#10498)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskCurableByStateId, dll_base + 0x00074EC0);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;

				// Call both implementations
				const auto moo_result = sut(nState);
				const auto original_result = original(nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4F00 (#10554)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskStayDeathOnUnitByStateId, dll_base + 0x00074F00);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;
				// Call both implementations
				const auto moo_result = sut(&moo_pUnit, nState);
				const auto original_result = original(&original_pUnit, nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4F60")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRestrictOnUnit, dll_base + 0x00074F60);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nullptr);
			const auto original_result = original(&original_pUnit, nullptr);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB4FC0 (#10536)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskBlueOnUnit, dll_base + 0x00074FC0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5030 (#10539)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskArmBlueOnUnit, dll_base + 0x00075030);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB50A0 (#10540)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRFBlueOnUnit, dll_base + 0x000750A0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5110 (#10541)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRCBlueOnUnit, dll_base + 0x00075110);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5180 (#10542)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRLBlueOnUnit, dll_base + 0x00075180);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB51F0 (#10543)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRPBlueOnUnit, dll_base + 0x000751F0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5260 (#10546)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskArmRedOnUnit, dll_base + 0x00075260);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB52D0 (#10547)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRFRedOnUnit, dll_base + 0x000752D0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5340 (#10548)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRCRedOnUnit, dll_base + 0x00075340);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB53B0 (#10549)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRLRedOnUnit, dll_base + 0x000753B0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5420 (#10550)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskRPRedOnUnit, dll_base + 0x00075420);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5490 (#10552)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskExpOnUnit, dll_base + 0x00075490);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<NoOpFixture>, "D2Common.0x6FDB5500 (#10553)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskExpByStateId, dll_base + 0x00075500);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;

				// Call both implementations
				const auto moo_result = sut(nState);
				const auto original_result = original(nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5540 (#10555)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskShatterOnUnit, dll_base + 0x00075540);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<NoOpFixture>, "D2Common.0x6FDB55B0 (#10556)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskShatterByStateId, dll_base + 0x000755B0);

		SUBCASE("")
		{
			for (auto i = 0; i < states_record_count; ++i)
			{
				int nState = i;

				// Call both implementations
				const auto moo_result = sut(nState);
				const auto original_result = original(nState);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB55F0 (#10559)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskLifeOnUnit, dll_base + 0x000755F0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5660 (#10560)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskUdeadOnUnit, dll_base + 0x00075660);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB56D0 (#10561)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskGreenOnUnit, dll_base + 0x000756D0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}

	TEST_CASE_FIXTURE(UnitWithStatFlagsFixture<StatesTxtFixture<NoOpFixture>>, "D2Common.0x6FDB5740 (#11303)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATES_CheckStateMaskOnUnit, dll_base + 0x00075740);

		REPEAT_10();

		SUBCASE("")
		{
			for (auto i = 0; i < 40; ++i)
			{
				int nStateMask = i;

				reroll_flags();

				// Call both implementations
				const auto moo_result = sut(&moo_pUnit, nStateMask);
				const auto original_result = original(&original_pUnit, nStateMask);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			}
		}
	}
}

#endif
