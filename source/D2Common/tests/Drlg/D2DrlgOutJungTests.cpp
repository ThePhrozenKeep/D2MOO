#include <D2CommonTestDefines.h>

#ifdef DRLG_OUTJUNG_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgOutJung.h>


TEST_SUITE("D2DrlgOutJungTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD7FC20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTJUNG_BuildJungle, dll_base + 0x0003FC20);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			sut(&moo_pLevel);
			original(&original_pLevel);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD7FE50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTJUNG_BuildLowerKurast, dll_base + 0x0003FE50);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			sut(&moo_pLevel);
			original(&original_pLevel);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD7FFA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTJUNG_BuildKurastBazaar, dll_base + 0x0003FFA0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			sut(&moo_pLevel);
			original(&original_pLevel);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD800E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTJUNG_BuildUpperKurast, dll_base + 0x000400E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			sut(&moo_pLevel);
			original(&original_pLevel);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD80230" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTJUNG_SpawnRandomPreset, dll_base + 0x00040230);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};
			int nLevelPrestId1{};
			int nLevelPrestId2{};
			int a4{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			sut(&moo_pLevel, nLevelPrestId1, nLevelPrestId2, a4);
			original(&original_pLevel, nLevelPrestId1, nLevelPrestId2, a4);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
}
#endif
