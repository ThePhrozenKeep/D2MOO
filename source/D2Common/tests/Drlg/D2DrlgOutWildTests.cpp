#include <D2CommonTestDefines.h>

#ifdef DRLG_OUTWILD_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgOutWild.h>


TEST_SUITE("D2DrlgOutWildTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD84CA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTWILD_GetBridgeCoords, dll_base + 0x00044CA0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			int moo_pX{};
			int moo_pY{};
			D2DrlgLevelStrc original_pLevel{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel,
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel, moo_pX, moo_pY);
			setup_data(original_pLevel, original_pX, original_pY);

			// Call both implementations
			sut(&moo_pLevel, &moo_pX, &moo_pY);
			original(&original_pLevel, &original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD84D30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTWILD_InitAct1OutdoorLevel, dll_base + 0x00044D30);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD85060" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTWILD_TestSpawnRiver, dll_base + 0x00045060);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};
			int nX{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevel, nX);
			const auto original_result = original(&original_pLevel, nX);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD850B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTWILD_SpawnRiver, dll_base + 0x000450B0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};
			int nX{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			sut(&moo_pLevel, nX);
			original(&original_pLevel, nX);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD85300" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD85300, dll_base + 0x00045300);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgVertexStrc moo_pDrlgVertex{};
			D2DrlgVertexStrc original_pDrlgVertex{};

			const auto setup_data = [](
				D2DrlgVertexStrc& pDrlgVertex
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgVertex);
			setup_data(original_pDrlgVertex);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgVertex);
			const auto original_result = original(&original_pDrlgVertex);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgVertex, original_pDrlgVertex, "Comparing pDrlgVertex");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD85350" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD85350, dll_base + 0x00045350);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgVertexStrc moo_pDrlgVertex{};
			D2DrlgVertexStrc original_pDrlgVertex{};

			const auto setup_data = [](
				D2DrlgVertexStrc& pDrlgVertex
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgVertex);
			setup_data(original_pDrlgVertex);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgVertex);
			const auto original_result = original(&original_pDrlgVertex);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgVertex, original_pDrlgVertex, "Comparing pDrlgVertex");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD85390" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTWILD_SpawnCliffCaves, dll_base + 0x00045390);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevel, nX, nY);
			const auto original_result = original(&original_pLevel, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD853F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTWILD_SpawnTownTransitionsAndCaves, dll_base + 0x000453F0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD85520" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTWILD_SpawnSpecialPresets, dll_base + 0x00045520);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD85920" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTWILD_SpawnCottage, dll_base + 0x00045920);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};
			int nLvlPrestId{};
			int a3{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			sut(&moo_pLevel, nLvlPrestId, a3);
			original(&original_pLevel, nLvlPrestId, a3);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
}
#endif
