#include <D2CommonTestDefines.h>

#ifdef DRLG_OUTPLACE_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgOutPlace.h>


TEST_SUITE("D2DrlgOutPlaceTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD80480" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_BuildKurast, dll_base + 0x00040480);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD806A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_InitAct3OutdoorLevel, dll_base + 0x000406A0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD80750" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD80750, dll_base + 0x00040750);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};
			int nVertexId{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevel, nVertexId);
			const auto original_result = original(&original_pLevel, nVertexId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD80BE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD80BE0, dll_base + 0x00040BE0);
		
		SUBCASE("")
		{
			int a1{};
			int a2{};
			int a3{};

			// Call both implementations
			const auto moo_result = sut(a1, a2, a3);
			const auto original_result = original(a1, a2, a3);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD80C10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD80C10, dll_base + 0x00040C10);
		
		SUBCASE("")
		{
			int a1{};
			int a2{};
			int a3{};
			int a4{};
			int a5{};

			// Call both implementations
			const auto moo_result = sut(a1, a2, a3, a4, a5);
			const auto original_result = original(a1, a2, a3, a4, a5);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD80C80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_SetBlankBorderGridCells, dll_base + 0x00040C80);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD80DA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_SetOutGridLinkFlags, dll_base + 0x00040DA0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD80E10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_PlaceAct1245OutdoorBorders, dll_base + 0x00040E10);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81330" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81330, dll_base + 0x00041330);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData);
			const auto original_result = original(&original_pLevelLinkData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81380" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81380, dll_base + 0x00041380);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData);
			const auto original_result = original(&original_pLevelLinkData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81430" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81430, dll_base + 0x00041430);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordStrc moo_pDrlgCoord1{};
			D2DrlgCoordStrc moo_pDrlgCoord2{};
			D2DrlgCoordStrc original_pDrlgCoord1{};
			D2DrlgCoordStrc original_pDrlgCoord2{};
			int a3{};
			int a4{};

			const auto setup_data = [](
				D2DrlgCoordStrc& pDrlgCoord1,
				D2DrlgCoordStrc& pDrlgCoord2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoord1, moo_pDrlgCoord2);
			setup_data(original_pDrlgCoord1, original_pDrlgCoord2);

			// Call both implementations
			sut(&moo_pDrlgCoord1, &moo_pDrlgCoord2, a3, a4);
			original(&original_pDrlgCoord1, &original_pDrlgCoord2, a3, a4);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord1, original_pDrlgCoord1, "Comparing pDrlgCoord1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord2, original_pDrlgCoord2, "Comparing pDrlgCoord2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81530" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81530, dll_base + 0x00041530);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData);
			const auto original_result = original(&original_pLevelLinkData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD815E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD815E0, dll_base + 0x000415E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordStrc moo_pDrlgCoord1{};
			D2DrlgCoordStrc moo_pDrlgCoord2{};
			D2DrlgCoordStrc original_pDrlgCoord1{};
			D2DrlgCoordStrc original_pDrlgCoord2{};
			int a3{};
			int a4{};

			const auto setup_data = [](
				D2DrlgCoordStrc& pDrlgCoord1,
				D2DrlgCoordStrc& pDrlgCoord2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoord1, moo_pDrlgCoord2);
			setup_data(original_pDrlgCoord1, original_pDrlgCoord2);

			// Call both implementations
			sut(&moo_pDrlgCoord1, &moo_pDrlgCoord2, a3, a4);
			original(&original_pDrlgCoord1, &original_pDrlgCoord2, a3, a4);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord1, original_pDrlgCoord1, "Comparing pDrlgCoord1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord2, original_pDrlgCoord2, "Comparing pDrlgCoord2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81720" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81720, dll_base + 0x00041720);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData);
			const auto original_result = original(&original_pLevelLinkData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81850" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81850, dll_base + 0x00041850);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordStrc moo_pDrlgCoord1{};
			D2DrlgCoordStrc moo_pDrlgCoord2{};
			D2DrlgCoordStrc original_pDrlgCoord1{};
			D2DrlgCoordStrc original_pDrlgCoord2{};
			int a3{};
			int a4{};

			const auto setup_data = [](
				D2DrlgCoordStrc& pDrlgCoord1,
				D2DrlgCoordStrc& pDrlgCoord2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoord1, moo_pDrlgCoord2);
			setup_data(original_pDrlgCoord1, original_pDrlgCoord2);

			// Call both implementations
			sut(&moo_pDrlgCoord1, &moo_pDrlgCoord2, a3, a4);
			original(&original_pDrlgCoord1, &original_pDrlgCoord2, a3, a4);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord1, original_pDrlgCoord1, "Comparing pDrlgCoord1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord2, original_pDrlgCoord2, "Comparing pDrlgCoord2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81950" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81950, dll_base + 0x00041950);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData);
			const auto original_result = original(&original_pLevelLinkData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81AD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81AD0, dll_base + 0x00041AD0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData);
			const auto original_result = original(&original_pLevelLinkData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81B30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81B30, dll_base + 0x00041B30);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData);
			const auto original_result = original(&original_pLevelLinkData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81BF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81BF0, dll_base + 0x00041BF0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData);
			const auto original_result = original(&original_pLevelLinkData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81CA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD81CA0, dll_base + 0x00041CA0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData);
			const auto original_result = original(&original_pLevelLinkData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD81D60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_CreateLevelConnections, dll_base + 0x00041D60);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgStrc original_pDrlg{};
			uint8_t nActNo{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg);
			setup_data(original_pDrlg);

			// Call both implementations
			sut(&moo_pDrlg, nActNo);
			original(&original_pDrlg, nActNo);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD82050" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD82050, dll_base + 0x00042050);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};
			int nIteration{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData, nIteration);
			const auto original_result = original(&original_pLevelLinkData, nIteration);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD82130" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD82130, dll_base + 0x00042130);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};
			int nIteration{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData, nIteration);
			const auto original_result = original(&original_pLevelLinkData, nIteration);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD821E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_LinkAct2Outdoors, dll_base + 0x000421E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};
			int nIteration{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData, nIteration);
			const auto original_result = original(&original_pLevelLinkData, nIteration);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD82240" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_LinkAct2Canyon, dll_base + 0x00042240);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};
			int nIteration{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData, nIteration);
			const auto original_result = original(&original_pLevelLinkData, nIteration);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Comon.0x6FD822A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_LinkAct4Outdoors, dll_base + 0x000422A0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};
			int nIteration{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData, nIteration);
			const auto original_result = original(&original_pLevelLinkData, nIteration);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD82300" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_LinkAct4ChaosSanctum, dll_base + 0x00042300);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelLinkDataStrc moo_pLevelLinkData{};
			D2DrlgLevelLinkDataStrc original_pLevelLinkData{};
			int nIteration{};

			const auto setup_data = [](
				D2DrlgLevelLinkDataStrc& pLevelLinkData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelLinkData);
			setup_data(original_pLevelLinkData);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevelLinkData, nIteration);
			const auto original_result = original(&original_pLevelLinkData, nIteration);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelLinkData, original_pLevelLinkData, "Comparing pLevelLinkData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD82360" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD82360, dll_base + 0x00042360);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			int moo_pRand{};
			D2DrlgLevelStrc original_pLevel{};
			int original_pRand{};
			int nIteration{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel,
				int& pRand
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel, moo_pRand);
			setup_data(original_pLevel, original_pRand);

			// Call both implementations
			sut(&moo_pLevel, nIteration, &moo_pRand);
			original(&original_pLevel, nIteration, &original_pRand);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
			SKIP_MOO_CHECK_EQ(moo_pRand, original_pRand, "Comparing pRand");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD823C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD823C0, dll_base + 0x000423C0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgLinkStrc moo_pDrlgLink{};
			D2DrlgStrc original_pDrlg{};
			D2DrlgLinkStrc original_pDrlgLink{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg,
				D2DrlgLinkStrc& pDrlgLink
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg, moo_pDrlgLink);
			setup_data(original_pDrlg, original_pDrlgLink);

			// Call both implementations
			sut(&moo_pDrlg, &moo_pDrlgLink, nullptr, nullptr);
			original(&original_pDrlg, &original_pDrlgLink, nullptr, nullptr);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
			SKIP_MOO_CHECK_EQ(moo_pDrlgLink, original_pDrlgLink, "Comparing pDrlgLink");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD826D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD826D0, dll_base + 0x000426D0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgStrc original_pDrlg{};
			int nStartId{};
			int nEndId{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg);
			setup_data(original_pDrlg);

			// Call both implementations
			sut(&moo_pDrlg, nStartId, nEndId);
			original(&original_pDrlg, nStartId, nEndId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD82750" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD82750, dll_base + 0x00042750);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgStrc original_pDrlg{};
			int nStartId{};
			int nEndId{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg);
			setup_data(original_pDrlg);

			// Call both implementations
			sut(&moo_pDrlg, nStartId, nEndId);
			original(&original_pDrlg, nStartId, nEndId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD82820" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLG_GenerateJungles, dll_base + 0x00042820);
		
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
			const auto moo_result = sut(&moo_pLevel);
			const auto original_result = original(&original_pLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD83970" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD83970, dll_base + 0x00043970);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordStrc moo_pDrlgCoord{};
			D2JungleStrc moo_pJungle{};
			D2DrlgCoordStrc original_pDrlgCoord{};
			D2JungleStrc original_pJungle{};
			int nRand{};
			int nSizeX{};
			int nSizeY{};

			const auto setup_data = [](
				D2DrlgCoordStrc& pDrlgCoord,
				D2JungleStrc& pJungle
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoord, moo_pJungle);
			setup_data(original_pDrlgCoord, original_pJungle);

			// Call both implementations
			sut(&moo_pDrlgCoord, &moo_pJungle, nRand, nSizeX, nSizeY);
			original(&original_pDrlgCoord, &original_pJungle, nRand, nSizeX, nSizeY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord, original_pDrlgCoord, "Comparing pDrlgCoord");
			SKIP_MOO_CHECK_EQ(moo_pJungle, original_pJungle, "Comparing pJungle");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD83A20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_InitOutdoorRoomGrids, dll_base + 0x00043A20);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom);
			original(&original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD83C90" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTPLACE_CreateOutdoorRoomEx, dll_base + 0x00043C90);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};
			int nX{};
			int nY{};
			int nWidth{};
			int nHeight{};
			int dwRoomFlags{};
			int dwOutdoorFlags{};
			int dwOutdoorFlagsEx{};
			int dwDT1Mask{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			sut(&moo_pLevel, nX, nY, nWidth, nHeight, dwRoomFlags, dwOutdoorFlags, dwOutdoorFlagsEx, dwDT1Mask);
			original(&original_pLevel, nX, nY, nWidth, nHeight, dwRoomFlags, dwOutdoorFlags, dwOutdoorFlagsEx, dwDT1Mask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
}
#endif
