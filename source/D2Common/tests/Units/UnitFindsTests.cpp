#include <D2CommonTestDefines.h>

#ifdef UNITFINDS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlg.h>
#include <Units/UnitFinds.h>
#include <Units/Units.h>


TEST_SUITE("UnitFindsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC680 (#10408)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITFINDS_AreUnitsInNeighboredRooms, dll_base + 0x0007C680);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pDestUnit{};
			D2UnitStrc moo_pSrcUnit{};
			D2UnitStrc original_pDestUnit{};
			D2UnitStrc original_pSrcUnit{};

			const auto setup_data = [](
				D2UnitStrc& pDestUnit,
				D2UnitStrc& pSrcUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDestUnit, moo_pSrcUnit);
			setup_data(original_pDestUnit, original_pSrcUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pDestUnit, &moo_pSrcUnit);
			const auto original_result = original(&original_pDestUnit, &original_pSrcUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDestUnit, original_pDestUnit, "Comparing pDestUnit");
			SKIP_MOO_CHECK_EQ(moo_pSrcUnit, original_pSrcUnit, "Comparing pSrcUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC720 (#11087)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITFINDS_FindUnitInNeighboredRooms, dll_base + 0x0007C720);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nUnitType{};
			int nClassId{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nUnitType, nClassId);
			const auto original_result = original(&original_pRoom, nUnitType, nClassId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC7B0 (#10405)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITFINDS_GetTestedUnitsFromRoom, dll_base + 0x0007C7B0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2UnitStrc* moo_ppUnits{};
			D2UnitFindArgStrc moo_pUnitFindArg{};
			D2ActiveRoomStrc original_pRoom{};
			D2UnitStrc* original_ppUnits{};
			D2UnitFindArgStrc original_pUnitFindArg{};
			UNITFINDTEST pfnUnitTest{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2UnitStrc*& ppUnits,
				D2UnitFindArgStrc& pUnitFindArg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_ppUnits, moo_pUnitFindArg);
			setup_data(original_pRoom, original_ppUnits, original_pUnitFindArg);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_ppUnits, pfnUnitTest, &moo_pUnitFindArg);
			const auto original_result = original(&original_pRoom, &original_ppUnits, pfnUnitTest, &original_pUnitFindArg);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_ppUnits, original_ppUnits, "Comparing ppUnits");
			SKIP_MOO_CHECK_EQ(moo_pUnitFindArg, original_pUnitFindArg, "Comparing pUnitFindArg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC840 (#11088)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITFINDS_GetNearestTestedUnit, dll_base + 0x0007C840);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nX{};
			int nY{};
			int nSize{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nX, nY, nSize, nullptr);
			const auto original_result = original(&original_pUnit, nX, nY, nSize, nullptr);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC990 (#10401)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITFINDS_InitializeUnitFindData, dll_base + 0x0007C990);
		
		SUBCASE("")
		{
			// Input data
			D2UnitFindDataStrc moo_pUnitFindData{};
			D2ActiveRoomStrc moo_pRoom{};
			D2UnitFindArgStrc moo_pUnitFindArg{};
			D2UnitFindDataStrc original_pUnitFindData{};
			D2ActiveRoomStrc original_pRoom{};
			D2UnitFindArgStrc original_pUnitFindArg{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nX{};
			int nY{};
			int nSize{};
			UNITFINDTEST pfnUnitTest{};

			const auto setup_data = [](
				D2UnitFindDataStrc& pUnitFindData,
				D2ActiveRoomStrc& pRoom,
				D2UnitFindArgStrc& pUnitFindArg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnitFindData, moo_pRoom, moo_pUnitFindArg);
			setup_data(original_pUnitFindData, original_pRoom, original_pUnitFindArg);

			// Call both implementations
			sut(moo_pMemPool, &moo_pUnitFindData, &moo_pRoom, nX, nY, nSize, pfnUnitTest, &moo_pUnitFindArg);
			original(original_pMemPool, &original_pUnitFindData, &original_pRoom, nX, nY, nSize, pfnUnitTest, &original_pUnitFindArg);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pUnitFindData, original_pUnitFindData, "Comparing pUnitFindData");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pUnitFindArg, original_pUnitFindArg, "Comparing pUnitFindArg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBCA50 (#10402)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITFINDS_FreeUnitFindData, dll_base + 0x0007CA50);
		
		SUBCASE("")
		{
			// Input data
			D2UnitFindDataStrc moo_pUnitFindData{};
			D2UnitFindDataStrc original_pUnitFindData{};

			const auto setup_data = [](
				D2UnitFindDataStrc& pUnitFindData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnitFindData);
			setup_data(original_pUnitFindData);

			// Call both implementations
			sut(&moo_pUnitFindData);
			original(&original_pUnitFindData);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnitFindData, original_pUnitFindData, "Comparing pUnitFindData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBCA80 (#10403)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITFINDS_FindAllMatchingUnitsInNeighboredRooms, dll_base + 0x0007CA80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitFindDataStrc moo_pUnitFindData{};
			D2UnitFindDataStrc original_pUnitFindData{};

			const auto setup_data = [](
				D2UnitFindDataStrc& pUnitFindData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnitFindData);
			setup_data(original_pUnitFindData);

			// Call both implementations
			sut(&moo_pUnitFindData);
			original(&original_pUnitFindData);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnitFindData, original_pUnitFindData, "Comparing pUnitFindData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBCCA0 (#10404)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITFINDS_TestUnit, dll_base + 0x0007CCA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitFindArgStrc moo_pUnitFindArg{};
			D2UnitStrc original_pUnit{};
			D2UnitFindArgStrc original_pUnitFindArg{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitFindArgStrc& pUnitFindArg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pUnitFindArg);
			setup_data(original_pUnit, original_pUnitFindArg);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pUnitFindArg);
			const auto original_result = original(&original_pUnit, &original_pUnitFindArg);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pUnitFindArg, original_pUnitFindArg, "Comparing pUnitFindArg");
		}
	}
}
#endif
