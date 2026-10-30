#include <D2CommonTestDefines.h>

#ifdef SEED_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Seed.h>


TEST_SUITE("D2SeedTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5260 (#10916)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SEED_Return, dll_base + 0x00065260);

		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();

			// NOTE: Nothing to test. The function just returns
		}
	}

	// NOTE: We're checking successful repetitions here, because a single run could fail due to the function calling GetTickCount() and time()
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEA80 (#10920)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SEED_GetRandomValue, dll_base + 0x0006EA80);

		SUBCASE("")
		{
			const auto repetitions = 100;
			auto success_count = 0;

			for (auto i = 0; i < repetitions; ++i)
			{
				int nValue = random_unsigned_integer();

				// Call both implementations
				const auto moo_result = sut(nValue);
				const auto original_result = original(nValue);

				// Compare return values
				std::string diff_description;
				const auto are_objects_equal = moo_check_eq(moo_result, original_result, "Comparing results", diff_description);
				if (are_objects_equal)
				{
					++success_count;
				}
			}

			CHECK(success_count > 95 * repetitions / 100);
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEAB0 (#10912)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SEED_InitSeed, dll_base + 0x0006EAB0);

		SUBCASE("")
		{
			// Input data
			D2SeedStrc moo_pSeed{};
			D2SeedStrc original_pSeed{};

			// Call both implementations
			sut(&moo_pSeed);
			original(&original_pSeed);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pSeed, original_pSeed, "Comparing pSeed");

			// Check specific values
			CHECK_EQ(moo_pSeed.nLowSeed, 1);
			CHECK_EQ(moo_pSeed.nHighSeed, 666);
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEAC0 (#10913)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SEED_InitLowSeed, dll_base + 0x0006EAC0);

		SUBCASE("")
		{
			// Input data
			D2SeedStrc moo_pSeed{};
			D2SeedStrc original_pSeed{};
			int nLowSeed = random_unsigned_integer();

			// Call both implementations
			sut(&moo_pSeed, nLowSeed);
			original(&original_pSeed, nLowSeed);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pSeed, original_pSeed, "Comparing pSeed");

			// Check specific values
			CHECK_EQ(moo_pSeed.nLowSeed, nLowSeed);
			CHECK_EQ(moo_pSeed.nHighSeed, 666);
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEAD0 (#10914)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SEED_GetLowSeed, dll_base + 0x0006EAD0);

		SUBCASE("")
		{
			// Input data
			const auto low_seed = random_unsigned_integer();
			const auto high_seed = random_unsigned_integer();

			D2SeedStrc moo_pSeed{};
			D2SeedStrc original_pSeed{};

			const auto setup_data = [low_seed, high_seed](
				D2SeedStrc& pSeed
			) {
				pSeed.nLowSeed = low_seed;
				pSeed.nHighSeed = high_seed;
			};

			setup_data(moo_pSeed);
			setup_data(original_pSeed);

			// Call both implementations
			const auto moo_result = sut(&moo_pSeed);
			const auto original_result = original(&original_pSeed);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pSeed, original_pSeed, "Comparing pSeed");

			// Check specific values
			CHECK_EQ(moo_result, low_seed);
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEAE0 (#10921)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SEED_SetSeeds, dll_base + 0x0006EAE0);

		SUBCASE("")
		{
			// Input data
			D2SeedStrc moo_pSeed{};
			D2SeedStrc original_pSeed{};
			uint32_t nLowSeed = random_unsigned_integer();
			uint32_t nHighSeed = random_unsigned_integer();

			// Call both implementations
			sut(&moo_pSeed, nLowSeed, nHighSeed);
			original(&original_pSeed, nLowSeed, nHighSeed);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pSeed, original_pSeed, "Comparing pSeed");

			// Check specific values
			CHECK_EQ(moo_pSeed.nLowSeed, nLowSeed);
			CHECK_EQ(moo_pSeed.nHighSeed, nHighSeed);
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEAF0 (#10922)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SEED_GetSeeds, dll_base + 0x0006EAF0);

		SUBCASE("")
		{
			// Input data
			const auto low_seed = random_unsigned_integer();
			const auto high_seed = random_unsigned_integer();

			D2SeedStrc moo_pSeed{};
			uint32_t moo_pLowSeed{};
			uint32_t moo_pHighSeed{};
			D2SeedStrc original_pSeed{};
			uint32_t original_pLowSeed{};
			uint32_t original_pHighSeed{};

			const auto setup_data = [low_seed, high_seed](
				D2SeedStrc& pSeed,
				uint32_t& pLowSeed,
				uint32_t& pHighSeed
			) {
				pSeed.nLowSeed = low_seed;
				pSeed.nHighSeed = high_seed;
			};

			setup_data(moo_pSeed, moo_pLowSeed, moo_pHighSeed);
			setup_data(original_pSeed, original_pLowSeed, original_pHighSeed);

			// Call both implementations
			sut(&moo_pSeed, &moo_pLowSeed, &moo_pHighSeed);
			original(&original_pSeed, &original_pLowSeed, &original_pHighSeed);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pSeed, original_pSeed, "Comparing pSeed");
			MOO_CHECK_EQ(moo_pLowSeed, original_pLowSeed, "Comparing pLowSeed");
			MOO_CHECK_EQ(moo_pHighSeed, original_pHighSeed, "Comparing pHighSeed");

			// Check specific values
			CHECK_EQ(moo_pLowSeed, moo_pSeed.nLowSeed);
			CHECK_EQ(moo_pHighSeed, moo_pSeed.nHighSeed);
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEB00 (#10915)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SEED_GetHighSeed, dll_base + 0x0006EB00);

		SUBCASE("")
		{
			// Input data
			const auto low_seed = random_unsigned_integer();
			const auto high_seed = random_unsigned_integer();

			D2SeedStrc moo_pSeed{};
			D2SeedStrc original_pSeed{};

			const auto setup_data = [low_seed, high_seed](
				D2SeedStrc& pSeed
			) {
				pSeed.nLowSeed = low_seed;
				pSeed.nHighSeed = high_seed;
			};

			setup_data(moo_pSeed);
			setup_data(original_pSeed);

			// Call both implementations
			const auto moo_result = sut(&moo_pSeed);
			const auto original_result = original(&original_pSeed);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pSeed, original_pSeed, "Comparing pSeed");

			// Check specific values
			CHECK_EQ(moo_result, moo_pSeed.nHighSeed);
		}
	}
}

#endif
