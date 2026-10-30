#include <D2CommonTestDefines.h>

#ifdef DRLG_ACTIVATE_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgActivate.h>
#include <Drlg/D2DrlgDrlg.h>


TEST_SUITE("D2DrlgActivateTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD733D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_RoomExSetStatus_ClientInRoom, dll_base + 0x000333D0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73450" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_RoomExSetStatus_ClientInSight, dll_base + 0x00033450);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73550" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_RoomExSetStatus_ClientOutOfSight, dll_base + 0x00033550);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD736F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_RoomExSetStatus_Untile, dll_base + 0x000336F0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73790" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_RoomExIdentifyRealStatus, dll_base + 0x00033790);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73880" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_RoomExStatusUnset_Untile, dll_base + 0x00033880);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD739A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_SetClientIsInSight, dll_base + 0x000339A0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgRoomStrc moo_pDrlgRoomHint{};
			D2DrlgStrc original_pDrlg{};
			D2DrlgRoomStrc original_pDrlgRoomHint{};
			int nLevelId{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg,
				D2DrlgRoomStrc& pDrlgRoomHint
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg, moo_pDrlgRoomHint);
			setup_data(original_pDrlg, original_pDrlgRoomHint);

			// Call both implementations
			sut(&moo_pDrlg, nLevelId, nX, nY, &moo_pDrlgRoomHint);
			original(&original_pDrlg, nLevelId, nX, nY, &original_pDrlgRoomHint);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoomHint, original_pDrlgRoomHint, "Comparing pDrlgRoomHint");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73A30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_RoomExPropagateSetStatus, dll_base + 0x00033A30);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			uint8_t nStatus{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgRoom, nStatus);
			original(original_pMemPool, &original_pDrlgRoom, nStatus);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73B40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_UnsetClientIsInSight, dll_base + 0x00033B40);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgRoomStrc moo_pDrlgRoomHint{};
			D2DrlgStrc original_pDrlg{};
			D2DrlgRoomStrc original_pDrlgRoomHint{};
			int nLevelId{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg,
				D2DrlgRoomStrc& pDrlgRoomHint
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg, moo_pDrlgRoomHint);
			setup_data(original_pDrlg, original_pDrlgRoomHint);

			// Call both implementations
			sut(&moo_pDrlg, nLevelId, nX, nY, &moo_pDrlgRoomHint);
			original(&original_pDrlg, nLevelId, nX, nY, &original_pDrlgRoomHint);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoomHint, original_pDrlgRoomHint, "Comparing pDrlgRoomHint");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73BE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_RoomExPropagateUnsetStatus, dll_base + 0x00033BE0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			uint8_t nStatus{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom, nStatus);
			original(&original_pDrlgRoom, nStatus);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73C40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_ChangeClientRoom, dll_base + 0x00033C40);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pPreviousRoom{};
			D2DrlgRoomStrc moo_pNewRoom{};
			D2DrlgRoomStrc original_pPreviousRoom{};
			D2DrlgRoomStrc original_pNewRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pPreviousRoom,
				D2DrlgRoomStrc& pNewRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPreviousRoom, moo_pNewRoom);
			setup_data(original_pPreviousRoom, original_pNewRoom);

			// Call both implementations
			sut(&moo_pPreviousRoom, &moo_pNewRoom);
			original(&original_pPreviousRoom, &original_pNewRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPreviousRoom, original_pPreviousRoom, "Comparing pPreviousRoom");
			SKIP_MOO_CHECK_EQ(moo_pNewRoom, original_pNewRoom, "Comparing pNewRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73CF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_InitializeRoomEx, dll_base + 0x00033CF0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73D80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_StreamRoomAtCoords, dll_base + 0x00033D80);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgStrc original_pDrlg{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg);
			setup_data(original_pDrlg);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlg, nX, nY);
			const auto original_result = original(&original_pDrlg, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73E30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_InitializeRoomExStatusLists, dll_base + 0x00033E30);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgStrc original_pDrlg{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg);
			setup_data(original_pDrlg);

			// Call both implementations
			sut(&moo_pDrlg);
			original(&original_pDrlg);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73E60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_GetARoomInClientSight, dll_base + 0x00033E60);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgStrc original_pDrlg{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg);
			setup_data(original_pDrlg);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlg);
			const auto original_result = original(&original_pDrlg);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73E90" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_GetARoomInSightButWithoutClient, dll_base + 0x00033E90);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgStrc original_pDrlg{};
			D2DrlgRoomStrc original_pDrlgRoom{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg,
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg, moo_pDrlgRoom);
			setup_data(original_pDrlg, original_pDrlgRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlg, &moo_pDrlgRoom);
			const auto original_result = original(&original_pDrlg, &original_pDrlgRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73EF0 (#10015)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_GetRoomsAllocationStats, dll_base + 0x00033EF0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pOutStatsClientAllocatedRooms{};
			int moo_pOutStatsClientFreedRooms{};
			int moo_pOutStatsAllocatedRooms{};
			int moo_pOutStatsFreedRooms{};
			int original_pOutStatsClientAllocatedRooms{};
			int original_pOutStatsClientFreedRooms{};
			int original_pOutStatsAllocatedRooms{};
			int original_pOutStatsFreedRooms{};

			const auto setup_data = [](
				int& pOutStatsClientAllocatedRooms,
				int& pOutStatsClientFreedRooms,
				int& pOutStatsAllocatedRooms,
				int& pOutStatsFreedRooms
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pOutStatsClientAllocatedRooms, moo_pOutStatsClientFreedRooms, moo_pOutStatsAllocatedRooms, moo_pOutStatsFreedRooms);
			setup_data(original_pOutStatsClientAllocatedRooms, original_pOutStatsClientFreedRooms, original_pOutStatsAllocatedRooms, original_pOutStatsFreedRooms);

			// Call both implementations
			sut(&moo_pOutStatsClientAllocatedRooms, &moo_pOutStatsClientFreedRooms, &moo_pOutStatsAllocatedRooms, &moo_pOutStatsFreedRooms);
			original(&original_pOutStatsClientAllocatedRooms, &original_pOutStatsClientFreedRooms, &original_pOutStatsAllocatedRooms, &original_pOutStatsFreedRooms);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pOutStatsClientAllocatedRooms, original_pOutStatsClientAllocatedRooms, "Comparing pOutStatsClientAllocatedRooms");
			SKIP_MOO_CHECK_EQ(moo_pOutStatsClientFreedRooms, original_pOutStatsClientFreedRooms, "Comparing pOutStatsClientFreedRooms");
			SKIP_MOO_CHECK_EQ(moo_pOutStatsAllocatedRooms, original_pOutStatsAllocatedRooms, "Comparing pOutStatsAllocatedRooms");
			SKIP_MOO_CHECK_EQ(moo_pOutStatsFreedRooms, original_pOutStatsFreedRooms, "Comparing pOutStatsFreedRooms");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD73F20 (#10003)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_Update, dll_base + 0x00033F20);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgStrc original_pDrlg{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg);
			setup_data(original_pDrlg);

			// Call both implementations
			sut(&moo_pDrlg);
			original(&original_pDrlg);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD74060" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_TestRoomCanUnTile, dll_base + 0x00034060);
		
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
			const auto moo_result = sut(&moo_pDrlgRoom);
			const auto original_result = original(&original_pDrlgRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD740F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_ToggleHasPortalFlag, dll_base + 0x000340F0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			BOOL bReset{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom, bReset);
			original(&original_pDrlgRoom, bReset);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD74110" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGACTIVATE_GetRoomStatusFlags, dll_base + 0x00034110);
		
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
			const auto moo_result = sut(&moo_pDrlgRoom);
			const auto original_result = original(&original_pDrlgRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
}
#endif
