#include <D2CommonTestDefines.h>

#ifdef ENVIRONMENT_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Environment.h>
#include <Drlg/D2DrlgDrlg.h>

#include <Fixtures/DataTbls/Fixtures.h>


TEST_SUITE("D2EnvironmentTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	// NOTE: may_fail because it uses GetTickCount() internally
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8D8E0" * doctest::may_fail())
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_AllocDrlgEnvironment, dll_base + 0x0004D8E0);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(nullptr);
			const auto original_result = original(nullptr);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}

	TEST_CASE_FIXTURE(LevelsTxtFixture<NoOpFixture>, "D2Common.0x6FD8D970")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_UpdateLightIntensity, dll_base + 0x0004D970);

		REPEAT_10();

		SUBCASE("")
		{
			const auto eclipse = GENERATE(0, 1);
			const auto time_rate = GENERATE(0, 4, 8, 128);

			for (auto j = 0; j < 5; ++j)
			{
				for (auto i = 0; i < levels_record_count; ++i)
				{
					// Input data
					const auto intensity = random_unsigned_integer(0, 255);
					const auto ticks = random_unsigned_integer(0, 1000);

					D2DrlgEnvironmentStrc moo_pEnvironment{};
					D2DrlgEnvironmentStrc original_pEnvironment{};
					int nLevelId = i;
					int nAct = j;

					const auto setup_data = [intensity, eclipse, ticks, time_rate](
						D2DrlgEnvironmentStrc& pEnvironment
						) {
							pEnvironment.nIntensity = intensity;
							pEnvironment.bEclipse = eclipse;
							pEnvironment.nTicks = ticks;
							pEnvironment.nTimeRate = time_rate;
						};

					setup_data(moo_pEnvironment);
					setup_data(original_pEnvironment);

					// Call both implementations
					sut(&moo_pEnvironment, nLevelId, nAct);
					original(&original_pEnvironment, nLevelId, nAct);

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pEnvironment, original_pEnvironment, "Comparing pEnvironment");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8DAC0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_UpdateLightColor, dll_base + 0x0004DAC0);

		SUBCASE("")
		{
			const auto eclipse = GENERATE(0, 1);
			const auto time_rate = GENERATE(0, 4, 8, 128);

			for (auto j = 0; j < 5; ++j)
			{
				for (auto i = 0; i < NUM_ENVIRONMENT_CYCLES; ++i)
				{
					// Input data
					const auto ticks = random_unsigned_integer(0, 1000);

					D2DrlgEnvironmentStrc moo_pEnvironment{};
					D2DrlgEnvironmentStrc original_pEnvironment{};
					int nAct = j;

					const auto setup_data = [i, eclipse, time_rate, ticks](
						D2DrlgEnvironmentStrc& pEnvironment
						) {
							pEnvironment.nCycleIndex = i;
							pEnvironment.bEclipse = eclipse;
							pEnvironment.nTimeRate = time_rate;
							pEnvironment.nTicks = ticks;
						};

					setup_data(moo_pEnvironment);
					setup_data(original_pEnvironment);

					// Call both implementations
					sut(&moo_pEnvironment, nAct);
					original(&original_pEnvironment, nAct);

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pEnvironment, original_pEnvironment, "Comparing pEnvironment");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(LevelsTxtFixture<NoOpFixture>, "D2Common.0x6FD8DC00 (#10923)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_UpdatePeriodOfDay, dll_base + 0x0004DC00);

		SUBCASE("")
		{
			for (auto i = 0; i < levels_record_count; ++i)
			{
				// Input data
				D2DrlgActStrc moo_pAct{};
				D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
				D2ActiveRoomStrc moo_pRoom{};
				D2DrlgRoomStrc moo_pDrlgRoom{};
				D2DrlgLevelStrc moo_pLevel{};
				D2DrlgActStrc original_pAct{};
				D2DrlgEnvironmentStrc original_pDrlgEnvironment{};
				D2ActiveRoomStrc original_pRoom{};
				D2DrlgRoomStrc original_pDrlgRoom{};
				D2DrlgLevelStrc original_pLevel{};

				const auto setup_data = [i](
					D2DrlgActStrc& pAct,
					D2DrlgEnvironmentStrc& pDrlgEnvironment,
					D2ActiveRoomStrc& pRoom,
					D2DrlgRoomStrc& pDrlgRoom,
					D2DrlgLevelStrc& pLevel
					) {
						pAct.pEnvironment = &pDrlgEnvironment;
						pRoom.pDrlgRoom = &pDrlgRoom;
						pDrlgRoom.pLevel = &pLevel;
						pLevel.nLevelId = i;

						// TODO: Set properties of pEnvironment
					};

				setup_data(moo_pAct, moo_pDrlgEnvironment, moo_pRoom, moo_pDrlgRoom, moo_pLevel);
				setup_data(original_pAct, original_pDrlgEnvironment, original_pRoom, original_pDrlgRoom, original_pLevel);

				// Call both implementations
				const auto moo_result = sut(&moo_pAct, &moo_pRoom);
				const auto original_result = original(&original_pAct, &original_pRoom);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
				MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8DC70")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_UpdateTicks, dll_base + 0x0004DC70);

		REPEAT_10();

		SUBCASE("")
		{
			const auto eclipse = GENERATE(0, 1);
			const auto time_rate = GENERATE(0, 4, 8, 128);

			for (auto cycle_index = 0; cycle_index < NUM_ENVIRONMENT_CYCLES; ++cycle_index)
			{
				for (auto act = 0; act < 5; ++act)
				{
					const auto ticks = random_unsigned_integer(0, 1000);

					// Input data
					D2DrlgEnvironmentStrc moo_pEnvironment{};
					D2DrlgEnvironmentStrc original_pEnvironment{};
					int nActNo = act;

					const auto setup_data = [cycle_index, ticks, time_rate, eclipse](
						D2DrlgEnvironmentStrc& pEnvironment
						) {
							pEnvironment.nCycleIndex = cycle_index;
							pEnvironment.nTicks = ticks;
							pEnvironment.nTimeRate = time_rate;
							pEnvironment.bEclipse = eclipse;
						};

					setup_data(moo_pEnvironment);
					setup_data(original_pEnvironment);

					// Call both implementations
					sut(&moo_pEnvironment, nActNo);
					original(&original_pEnvironment, nActNo);

					// Compare potentially modified input data
					MOO_CHECK_EQ(moo_pEnvironment, original_pEnvironment, "Comparing pEnvironment");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8DD60 (#10924)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_UpdateCycleIndex, dll_base + 0x0004DD60);

		SUBCASE("")
		{
			for (auto i = 0; i < 5; ++i)
			{
				// Input data
				D2DrlgActStrc moo_pAct{};
				D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
				D2DrlgActStrc original_pAct{};
				D2DrlgEnvironmentStrc original_pDrlgEnvironment{};
				int nActNo = i;

				const auto setup_data = [](
					D2DrlgActStrc& pAct,
					D2DrlgEnvironmentStrc& pDrlgEnvironment
					) {
						pAct.pEnvironment = &pDrlgEnvironment;

						// TODO: Set properties of pEnvironment
					};

				setup_data(moo_pAct, moo_pDrlgEnvironment);
				setup_data(original_pAct, original_pDrlgEnvironment);

				// Call both implementations
				const auto moo_result = sut(&moo_pAct, nActNo);
				const auto original_result = original(&original_pAct, nActNo);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8DDD0 (#10927)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_GetLightColorFromAct, dll_base + 0x0004DDD0);

		SUBCASE("")
		{
			// Input data
			const auto red = random_unsigned_integer(0, 255);
			const auto green = random_unsigned_integer(0, 255);
			const auto blue = random_unsigned_integer(0, 255);

			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			uint8_t moo_pRed{};
			uint8_t moo_pGreen{};
			uint8_t moo_pBlue{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};
			uint8_t original_pRed{};
			uint8_t original_pGreen{};
			uint8_t original_pBlue{};

			const auto setup_data = [red, green, blue](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment,
				uint8_t& pRed,
				uint8_t& pGreen,
				uint8_t& pBlue
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
					pDrlgEnvironment.nRed = red;
					pDrlgEnvironment.nGreen = green;
					pDrlgEnvironment.nBlue = blue;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment, moo_pRed, moo_pGreen, moo_pBlue);
			setup_data(original_pAct, original_pDrlgEnvironment, original_pRed, original_pGreen, original_pBlue);

			// Call both implementations
			sut(&moo_pAct, &moo_pRed, &moo_pGreen, &moo_pBlue);
			original(&original_pAct, &original_pRed, &original_pGreen, &original_pBlue);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			MOO_CHECK_EQ(moo_pRed, original_pRed, "Comparing pRed");
			MOO_CHECK_EQ(moo_pGreen, original_pGreen, "Comparing pGreen");
			MOO_CHECK_EQ(moo_pBlue, original_pBlue, "Comparing pBlue");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8DE00 (#10926)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_GetIntensityFromAct, dll_base + 0x0004DE00);

		SUBCASE("")
		{
			// Input data
			const auto intensity = random_unsigned_integer();

			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};

			const auto setup_data = [intensity](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
					pDrlgEnvironment.nIntensity = intensity;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment);
			setup_data(original_pAct, original_pDrlgEnvironment);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8DE20 (#10933)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_GetPeriodOfDayFromAct, dll_base + 0x0004DE20);

		SUBCASE("")
		{
			// Input data
			const auto time_rate = GENERATE(0, 4, 8, 128);
			const auto ticks = random_unsigned_integer();
			const auto period_of_day = random_unsigned_integer();

			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			int moo_pBaseTime{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};
			int original_pBaseTime{};

			const auto setup_data = [time_rate, ticks, period_of_day](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment,
				int& pBaseTime
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
					pDrlgEnvironment.nTimeRate = time_rate;
					pDrlgEnvironment.nTicks = ticks;
					pDrlgEnvironment.nPeriodOfDay = period_of_day;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment, moo_pBaseTime);
			setup_data(original_pAct, original_pDrlgEnvironment, original_pBaseTime);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct, &moo_pBaseTime);
			const auto original_result = original(&original_pAct, &original_pBaseTime);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			MOO_CHECK_EQ(moo_pBaseTime, original_pBaseTime, "Comparing pBaseTime");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8DE70 (#10928)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_GetUnusedMember, dll_base + 0x0004DE70);

		SUBCASE("")
		{
			// Input data
			const auto value = random_unsigned_integer();

			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};

			const auto setup_data = [value](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
					pDrlgEnvironment.unk0x14 = value;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment);
			setup_data(original_pAct, original_pDrlgEnvironment);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}

	TEST_CASE_FIXTURE(LevelsTxtFixture<NoOpFixture>, "D2Common.0x6FD8DE90 (#10929)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_NextEnvCycle, dll_base + 0x0004DE90);

		SUBCASE("")
		{
			for (auto i = 0; i < levels_record_count; ++i)
			{
				// Input data
				D2DrlgActStrc moo_pAct{};
				D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
				D2ActiveRoomStrc moo_pRoom{};
				D2DrlgRoomStrc moo_pDrlgRoom{};
				D2DrlgLevelStrc moo_pLevel{};
				D2DrlgActStrc original_pAct{};
				D2DrlgEnvironmentStrc original_pDrlgEnvironment{};
				D2ActiveRoomStrc original_pRoom{};
				D2DrlgRoomStrc original_pDrlgRoom{};
				D2DrlgLevelStrc original_pLevel{};

				const auto setup_data = [i](
					D2DrlgActStrc& pAct,
					D2DrlgEnvironmentStrc& pDrlgEnvironment,
					D2ActiveRoomStrc& pRoom,
					D2DrlgRoomStrc& pDrlgRoom,
					D2DrlgLevelStrc& pLevel
					) {
						pAct.pEnvironment = &pDrlgEnvironment;
						pRoom.pDrlgRoom = &pDrlgRoom;
						pDrlgRoom.pLevel = &pLevel;
						pLevel.nLevelId = i;

						// TODO: Set properties of pEnvironment
					};

				setup_data(moo_pAct, moo_pDrlgEnvironment, moo_pRoom, moo_pDrlgRoom, moo_pLevel);
				setup_data(original_pAct, original_pDrlgEnvironment, original_pRoom, original_pDrlgRoom, original_pLevel);

				// Call both implementations
				sut(&moo_pAct, &moo_pRoom);
				original(&original_pAct, &original_pRoom);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
				MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8DF30 (#10930)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_GetCycleIndexFromAct, dll_base + 0x0004DF30);

		SUBCASE("")
		{
			// Input data
			const auto cycle_index = random_unsigned_integer();

			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};

			const auto setup_data = [cycle_index](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
					pDrlgEnvironment.nCycleIndex = cycle_index;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment);
			setup_data(original_pAct, original_pDrlgEnvironment);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}

	TEST_CASE_FIXTURE(LevelsTxtFixture<NoOpFixture>, "D2Common.0x6FD8DF40 (#10932)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_InitializeEnvironment, dll_base + 0x0004DF40);

		SUBCASE("")
		{
			for (auto i = 0; i < levels_record_count; ++i)
			{
				// Input data
				D2DrlgActStrc moo_pAct{};
				D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
				D2ActiveRoomStrc moo_pRoom{};
				D2DrlgRoomStrc moo_pDrlgRoom{};
				D2DrlgLevelStrc moo_pLevel{};
				D2DrlgActStrc original_pAct{};
				D2DrlgEnvironmentStrc original_pDrlgEnvironment{};
				D2ActiveRoomStrc original_pRoom{};
				D2DrlgRoomStrc original_pDrlgRoom{};
				D2DrlgLevelStrc original_pLevel{};
				int nIndex{};
				int nTicks{};
				BOOL bEclipse{};

				const auto setup_data = [i](
					D2DrlgActStrc& pAct,
					D2DrlgEnvironmentStrc& pDrlgEnvironment,
					D2ActiveRoomStrc& pRoom,
					D2DrlgRoomStrc& pDrlgRoom,
					D2DrlgLevelStrc& pLevel
					) {
						pAct.pEnvironment = &pDrlgEnvironment;
						pRoom.pDrlgRoom = &pDrlgRoom;
						pDrlgRoom.pLevel = &pLevel;
						pLevel.nLevelId = i;

						// TODO: Set properties of pEnvironment
					};

				setup_data(moo_pAct, moo_pDrlgEnvironment, moo_pRoom, moo_pDrlgRoom, moo_pLevel);
				setup_data(original_pAct, original_pDrlgEnvironment, original_pRoom, original_pDrlgRoom, original_pLevel);

				// Call both implementations
				sut(&moo_pAct, &moo_pRoom, nIndex, nTicks, bEclipse);
				original(&original_pAct, &original_pRoom, nIndex, nTicks, bEclipse);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
				MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E080 (#10931)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_GetCycleIndex_Ticks_EclipseFromAct, dll_base + 0x0004E080);

		SUBCASE("")
		{
			const auto cycle_index = random_unsigned_integer();
			const auto ticks = random_unsigned_integer();
			const auto eclipse = random_unsigned_integer();

			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			int moo_pCycleIndex{};
			int moo_pTicks{};
			BOOL moo_pEclipse{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};
			int original_pCycleIndex{};
			int original_pTicks{};
			BOOL original_pEclipse{};

			const auto setup_data = [cycle_index, ticks, eclipse](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment,
				int& pCycleIndex,
				int& pTicks,
				BOOL& pEclipse
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
					pDrlgEnvironment.nCycleIndex = cycle_index;
					pDrlgEnvironment.nTicks = ticks;
					pDrlgEnvironment.bEclipse = eclipse;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment, moo_pCycleIndex, moo_pTicks, moo_pEclipse);
			setup_data(original_pAct, original_pDrlgEnvironment, original_pCycleIndex, original_pTicks, original_pEclipse);

			// Call both implementations
			sut(&moo_pAct, &moo_pCycleIndex, &moo_pTicks, &moo_pEclipse);
			original(&original_pAct, &original_pCycleIndex, &original_pTicks, &original_pEclipse);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			MOO_CHECK_EQ(moo_pCycleIndex, original_pCycleIndex, "Comparing pCycleIndex");
			MOO_CHECK_EQ(moo_pTicks, original_pTicks, "Comparing pTicks");
			MOO_CHECK_EQ(moo_pEclipse, original_pEclipse, "Comparing pEclipse");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E0B0 (#10925)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_GetStatistics, dll_base + 0x0004E0B0);

		SUBCASE("")
		{
			// Input data
			const auto cos = 0.0f;
			const auto sin = 1.0f;
			const auto last = 0.5f;

			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			float moo_pCos{};
			float moo_pLast{};
			float moo_pSin{};
			int moo_a5{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};
			float original_pCos{};
			float original_pLast{};
			float original_pSin{};
			int original_a5{};

			const auto setup_data = [cos, sin, last](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment,
				float& pCos,
				float& pLast,
				float& pSin,
				int& a5
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
					pDrlgEnvironment.fCos = cos;
					pDrlgEnvironment.fSin = sin;
					pDrlgEnvironment.fLast = last;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment, moo_pCos, moo_pLast, moo_pSin, moo_a5);
			setup_data(original_pAct, original_pDrlgEnvironment, original_pCos, original_pLast, original_pSin, original_a5);

			// Call both implementations
			sut(&moo_pAct, &moo_pCos, &moo_pLast, &moo_pSin, &moo_a5);
			original(&original_pAct, &original_pCos, &original_pLast, &original_pSin, &original_a5);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
			MOO_CHECK_EQ(moo_pCos, original_pCos, "Comparing pCos");
			MOO_CHECK_EQ(moo_pLast, original_pLast, "Comparing pLast");
			MOO_CHECK_EQ(moo_pSin, original_pSin, "Comparing pSin");
			MOO_CHECK_EQ(moo_a5, original_a5, "Comparing a5");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E0F0 (#10934)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_GetTimeRateFromAct, dll_base + 0x0004E0F0);

		SUBCASE("")
		{
			// Input data
			const auto time_rate = random_unsigned_integer();

			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};

			const auto setup_data = [time_rate](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
					pDrlgEnvironment.nTimeRate = time_rate;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment);
			setup_data(original_pAct, original_pDrlgEnvironment);

			// Call both implementations
			const auto moo_result = sut(&moo_pAct);
			const auto original_result = original(&original_pAct);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}

	TEST_CASE_FIXTURE(LevelsTxtFixture<NoOpFixture>, "D2Common.0x6FD8E100 (#10935)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_SetNextTimeRate, dll_base + 0x0004E100);

		SUBCASE("")
		{
			for (auto i = 0; i < levels_record_count; ++i)
			{
				// Input data
				D2DrlgActStrc moo_pAct{};
				D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
				D2ActiveRoomStrc moo_pRoom{};
				D2DrlgRoomStrc moo_pDrlgRoom{};
				D2DrlgLevelStrc moo_pLevel{};
				D2DrlgActStrc original_pAct{};
				D2DrlgEnvironmentStrc original_pDrlgEnvironment{};
				D2ActiveRoomStrc original_pRoom{};
				D2DrlgRoomStrc original_pDrlgRoom{};
				D2DrlgLevelStrc original_pLevel{};

				const auto setup_data = [i](
					D2DrlgActStrc& pAct,
					D2DrlgEnvironmentStrc& pDrlgEnvironment,
					D2ActiveRoomStrc& pRoom,
					D2DrlgRoomStrc& pDrlgRoom,
					D2DrlgLevelStrc& pLevel
					) {
						pAct.pEnvironment = &pDrlgEnvironment;
						pRoom.pDrlgRoom = &pDrlgRoom;
						pDrlgRoom.pLevel = &pLevel;
						pLevel.nLevelId = i;

						// TODO: Set properties of pEnvironment
					};

				setup_data(moo_pAct, moo_pDrlgEnvironment, moo_pRoom, moo_pDrlgRoom, moo_pLevel);
				setup_data(original_pAct, original_pDrlgEnvironment, original_pRoom, original_pDrlgRoom, original_pLevel);

				// Call both implementations
				sut(&moo_pAct, &moo_pRoom);
				original(&original_pAct, &original_pRoom);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
				MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E1B0 (#10936)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_TaintedSunBegin, dll_base + 0x0004E1B0);

		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment);
			setup_data(original_pAct, original_pDrlgEnvironment);

			// Call both implementations
			sut(&moo_pAct);
			original(&original_pAct);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E1E0 (#10937)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ENVIRONMENT_TaintedSunEnd, dll_base + 0x0004E1E0);

		SUBCASE("")
		{
			// Input data
			D2DrlgActStrc moo_pAct{};
			D2DrlgEnvironmentStrc moo_pDrlgEnvironment{};
			D2DrlgActStrc original_pAct{};
			D2DrlgEnvironmentStrc original_pDrlgEnvironment{};

			const auto setup_data = [](
				D2DrlgActStrc& pAct,
				D2DrlgEnvironmentStrc& pDrlgEnvironment
				) {
					pAct.pEnvironment = &pDrlgEnvironment;
				};

			setup_data(moo_pAct, moo_pDrlgEnvironment);
			setup_data(original_pAct, original_pDrlgEnvironment);

			// Call both implementations
			sut(&moo_pAct);
			original(&original_pAct);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pAct, original_pAct, "Comparing pAct");
		}
	}
}

#endif
