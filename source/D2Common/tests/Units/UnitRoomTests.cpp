#include <D2CommonTestDefines.h>

#ifdef UNITROOM_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlg.h>
#include <Units/UnitRoom.h>
#include <Units/Units.h>


TEST_SUITE("UnitRoomTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBCF10 (#11279)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_AddUnitToRoomEx, dll_base + 0x0007CF10);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2ActiveRoomStrc moo_pRoom{};
			D2UnitStrc original_pUnit{};
			D2ActiveRoomStrc original_pRoom{};
			int nUnused{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pRoom);
			setup_data(original_pUnit, original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pRoom, nUnused);
			const auto original_result = original(&original_pUnit, &original_pRoom, nUnused);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD100 (#10384)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_AddUnitToRoom, dll_base + 0x0007D100);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2ActiveRoomStrc moo_pRoom{};
			D2UnitStrc original_pUnit{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pRoom);
			setup_data(original_pUnit, original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pRoom);
			const auto original_result = original(&original_pUnit, &original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD120 (#10385)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_RefreshUnit, dll_base + 0x0007D120);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD1B0 (#10388)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_SortUnitListByTargetY, dll_base + 0x0007D1B0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom);
			original(&original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD250 (#10390)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_UpdatePath, dll_base + 0x0007D250);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD2B0 (#10391)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_ClearUpdateQueue, dll_base + 0x0007D2B0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			sut(&moo_pRoom);
			original(&original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD300 (#10386)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_RemoveUnitFromRoom, dll_base + 0x0007D300);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD400 (#10387)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_RemoveUnitFromUpdateQueue, dll_base + 0x0007D400);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD4C0 (#10389)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITROOM_IsUnitInRoom, dll_base + 0x0007D4C0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2UnitStrc moo_pUnit{};
			D2ActiveRoomStrc original_pRoom{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pUnit);
			setup_data(original_pRoom, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, &moo_pUnit);
			const auto original_result = original(&original_pRoom, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
}
#endif
