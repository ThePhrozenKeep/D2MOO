#include <D2CommonTestDefines.h>

#ifdef DRLG_OUTROOM_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgOutRoom.h>


TEST_SUITE("D2DrlgOutRoomTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD83D20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTROOM_FreeDrlgOutdoorRoom, dll_base + 0x00043D20);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD83D90" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTROOM_FreeDrlgOutdoorRoomData, dll_base + 0x00043D90);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD83DE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTROOM_AllocDrlgOutdoorRoom, dll_base + 0x00043DE0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD83EC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTROOM_LinkLevelsByLevelCoords, dll_base + 0x00043EC0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD83F70" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTROOM_LinkLevelsByLevelDef, dll_base + 0x00043F70);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD84010" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGOUTROOM_LinkLevelsByOffsetCoords, dll_base + 0x00044010);
		
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
}
#endif
