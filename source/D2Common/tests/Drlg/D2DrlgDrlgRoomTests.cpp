#include <D2CommonTestDefines.h>

#ifdef DRLG_ROOM_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgDrlgRoom.h>


TEST_SUITE("D2DrlgDrlgRoomTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD771C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_AllocRoomEx, dll_base + 0x000371C0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgLevelStrc original_pLevel{};
			int nType{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel);
			setup_data(original_pLevel);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevel, nType);
			const auto original_result = original(&original_pLevel, nType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77280" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD77280, dll_base + 0x00037280);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			BOOL bClient{};
			uint32_t nFlags{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom, bClient, nFlags);
			original(&original_pDrlgRoom, bClient, nFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD772B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_FreeRoomTiles, dll_base + 0x000372B0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgRoom);
			original(original_pMemPool, &original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD772F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_FreeRoomEx, dll_base + 0x000372F0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD774F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_FreeRoomData, dll_base + 0x000374F0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgOrthStrc moo_pDrlgRoomData{};
			D2DrlgOrthStrc original_pDrlgRoomData{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2DrlgOrthStrc& pDrlgRoomData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoomData);
			setup_data(original_pDrlgRoomData);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgRoomData);
			original(original_pMemPool, &original_pDrlgRoomData);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoomData, original_pDrlgRoomData, "Comparing pDrlgRoomData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77520" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_AllocDrlgOrthsForRooms, dll_base + 0x00037520);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom1{};
			D2DrlgRoomStrc moo_pDrlgRoom2{};
			D2DrlgRoomStrc original_pDrlgRoom1{};
			D2DrlgRoomStrc original_pDrlgRoom2{};
			int nDirection{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom1,
				D2DrlgRoomStrc& pDrlgRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom1, moo_pDrlgRoom2);
			setup_data(original_pDrlgRoom1, original_pDrlgRoom2);

			// Call both implementations
			sut(&moo_pDrlgRoom1, &moo_pDrlgRoom2, nDirection);
			original(&original_pDrlgRoom1, &original_pDrlgRoom2, nDirection);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom1, original_pDrlgRoom1, "Comparing pDrlgRoom1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom2, original_pDrlgRoom2, "Comparing pDrlgRoom2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77600" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_AddOrth, dll_base + 0x00037600);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgOrthStrc* moo_ppDrlgOrth{};
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgOrthStrc* original_ppDrlgOrth{};
			D2DrlgLevelStrc original_pLevel{};
			int nDirection{};
			BOOL bIsPreset{};

			const auto setup_data = [](
				D2DrlgOrthStrc*& ppDrlgOrth,
				D2DrlgLevelStrc& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_ppDrlgOrth, moo_pLevel);
			setup_data(original_ppDrlgOrth, original_pLevel);

			// Call both implementations
			sut(&moo_ppDrlgOrth, &moo_pLevel, nDirection, bIsPreset);
			original(&original_ppDrlgOrth, &original_pLevel, nDirection, bIsPreset);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_ppDrlgOrth, original_ppDrlgOrth, "Comparing ppDrlgOrth");
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD776B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD776B0, dll_base + 0x000376B0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgOrthStrc moo_pDrlgOrth1{};
			D2DrlgOrthStrc moo_pDrlgOrth2{};
			D2DrlgOrthStrc original_pDrlgOrth1{};
			D2DrlgOrthStrc original_pDrlgOrth2{};

			const auto setup_data = [](
				D2DrlgOrthStrc& pDrlgOrth1,
				D2DrlgOrthStrc& pDrlgOrth2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgOrth1, moo_pDrlgOrth2);
			setup_data(original_pDrlgOrth1, original_pDrlgOrth2);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgOrth1, &moo_pDrlgOrth2);
			const auto original_result = original(&original_pDrlgOrth1, &original_pDrlgOrth2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgOrth1, original_pDrlgOrth1, "Comparing pDrlgOrth1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgOrth2, original_pDrlgOrth2, "Comparing pDrlgOrth2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77740" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLG_GetRectanglesManhattanDistanceAndCheckNotOverlapping, dll_base + 0x00037740);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordStrc moo_pDrlgCoord1{};
			D2DrlgCoordStrc moo_pDrlgCoord2{};
			int moo_pDistanceX{};
			int moo_pDistanceY{};
			D2DrlgCoordStrc original_pDrlgCoord1{};
			D2DrlgCoordStrc original_pDrlgCoord2{};
			int original_pDistanceX{};
			int original_pDistanceY{};
			int nMaxDistance{};

			const auto setup_data = [](
				D2DrlgCoordStrc& pDrlgCoord1,
				D2DrlgCoordStrc& pDrlgCoord2,
				int& pDistanceX,
				int& pDistanceY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoord1, moo_pDrlgCoord2, moo_pDistanceX, moo_pDistanceY);
			setup_data(original_pDrlgCoord1, original_pDrlgCoord2, original_pDistanceX, original_pDistanceY);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgCoord1, &moo_pDrlgCoord2, nMaxDistance, &moo_pDistanceX, &moo_pDistanceY);
			const auto original_result = original(&original_pDrlgCoord1, &original_pDrlgCoord2, nMaxDistance, &original_pDistanceX, &original_pDistanceY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord1, original_pDrlgCoord1, "Comparing pDrlgCoord1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord2, original_pDrlgCoord2, "Comparing pDrlgCoord2");
			SKIP_MOO_CHECK_EQ(moo_pDistanceX, original_pDistanceX, "Comparing pDistanceX");
			SKIP_MOO_CHECK_EQ(moo_pDistanceY, original_pDistanceY, "Comparing pDistanceY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD777B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLG_CheckNotOverlappingUsingManhattanDistance, dll_base + 0x000377B0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordStrc moo_pDrlgCoord1{};
			D2DrlgCoordStrc moo_pDrlgCoord2{};
			D2DrlgCoordStrc original_pDrlgCoord1{};
			D2DrlgCoordStrc original_pDrlgCoord2{};
			int nMaxDistance{};

			const auto setup_data = [](
				D2DrlgCoordStrc& pDrlgCoord1,
				D2DrlgCoordStrc& pDrlgCoord2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoord1, moo_pDrlgCoord2);
			setup_data(original_pDrlgCoord1, original_pDrlgCoord2);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgCoord1, &moo_pDrlgCoord2, nMaxDistance);
			const auto original_result = original(&original_pDrlgCoord1, &original_pDrlgCoord2, nMaxDistance);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord1, original_pDrlgCoord1, "Comparing pDrlgCoord1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord2, original_pDrlgCoord2, "Comparing pDrlgCoord2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77800" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLG_CheckOverlappingWithOrthogonalMargin, dll_base + 0x00037800);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordStrc moo_pDrlgCoord1{};
			D2DrlgCoordStrc moo_pDrlgCoord2{};
			D2DrlgCoordStrc original_pDrlgCoord1{};
			D2DrlgCoordStrc original_pDrlgCoord2{};
			int nOrthogonalDistanceMax{};

			const auto setup_data = [](
				D2DrlgCoordStrc& pDrlgCoord1,
				D2DrlgCoordStrc& pDrlgCoord2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoord1, moo_pDrlgCoord2);
			setup_data(original_pDrlgCoord1, original_pDrlgCoord2);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgCoord1, &moo_pDrlgCoord2, nOrthogonalDistanceMax);
			const auto original_result = original(&original_pDrlgCoord1, &original_pDrlgCoord2, nOrthogonalDistanceMax);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord1, original_pDrlgCoord1, "Comparing pDrlgCoord1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord2, original_pDrlgCoord2, "Comparing pDrlgCoord2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77890" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGMAZE_CheckRoomNotOverlaping, dll_base + 0x00037890);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgRoomStrc moo_pDrlgRoom1{};
			D2DrlgRoomStrc moo_pIgnoredRoom{};
			D2DrlgLevelStrc original_pLevel{};
			D2DrlgRoomStrc original_pDrlgRoom1{};
			D2DrlgRoomStrc original_pIgnoredRoom{};
			int nMargin{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel,
				D2DrlgRoomStrc& pDrlgRoom1,
				D2DrlgRoomStrc& pIgnoredRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel, moo_pDrlgRoom1, moo_pIgnoredRoom);
			setup_data(original_pLevel, original_pDrlgRoom1, original_pIgnoredRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pLevel, &moo_pDrlgRoom1, &moo_pIgnoredRoom, nMargin);
			const auto original_result = original(&original_pLevel, &original_pDrlgRoom1, &original_pIgnoredRoom, nMargin);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom1, original_pDrlgRoom1, "Comparing pDrlgRoom1");
			SKIP_MOO_CHECK_EQ(moo_pIgnoredRoom, original_pIgnoredRoom, "Comparing pIgnoredRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77910" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_AddRoomExToLevel, dll_base + 0x00037910);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgLevelStrc moo_pLevel{};
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgLevelStrc original_pLevel{};
			D2DrlgRoomStrc original_pDrlgRoom{};

			const auto setup_data = [](
				D2DrlgLevelStrc& pLevel,
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevel, moo_pDrlgRoom);
			setup_data(original_pLevel, original_pDrlgRoom);

			// Call both implementations
			sut(&moo_pLevel, &moo_pDrlgRoom);
			original(&original_pLevel, &original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77930" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_AreXYInsideCoordinates, dll_base + 0x00037930);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordStrc moo_pDrlgCoord{};
			D2DrlgCoordStrc original_pDrlgCoord{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgCoordStrc& pDrlgCoord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoord);
			setup_data(original_pDrlgCoord);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgCoord, nX, nY);
			const auto original_result = original(&original_pDrlgCoord, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord, original_pDrlgCoord, "Comparing pDrlgCoord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77980" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_AreXYInsideCoordinatesOrOnBorder, dll_base + 0x00037980);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgCoordStrc moo_pDrlgCoord{};
			D2DrlgCoordStrc original_pDrlgCoord{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgCoordStrc& pDrlgCoord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgCoord);
			setup_data(original_pDrlgCoord);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgCoord, nX, nY);
			const auto original_result = original(&original_pDrlgCoord, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord, original_pDrlgCoord, "Comparing pDrlgCoord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD779D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_CheckLOSDraw, dll_base + 0x000379D0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD779F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD779F0, dll_base + 0x000379F0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77A00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_CheckWaypointFlags, dll_base + 0x00037A00);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77A10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_GetLevelId, dll_base + 0x00037A10);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77A20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_GetWarpDestinationLevel, dll_base + 0x00037A20);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			int nSourceLevel{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, nSourceLevel);
			const auto original_result = original(&original_pDrlgRoom, nSourceLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77AB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_GetLevelIdFromPopulatedRoom, dll_base + 0x00037AB0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77AF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_HasWaypoint, dll_base + 0x00037AF0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77B20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_GetPickedLevelPrestFilePathFromRoomEx, dll_base + 0x00037B20);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77B50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_ReorderNearRoomList, dll_base + 0x00037B50);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2ActiveRoomStrc* moo_ppRoomList{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2ActiveRoomStrc* original_ppRoomList{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2ActiveRoomStrc*& ppRoomList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_ppRoomList);
			setup_data(original_pDrlgRoom, original_ppRoomList);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, &moo_ppRoomList);
			const auto original_result = original(&original_pDrlgRoom, &original_ppRoomList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_ppRoomList, original_ppRoomList, "Comparing ppRoomList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77BB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD77BB0, dll_base + 0x00037BB0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgRoom);
			original(original_pMemPool, &original_pDrlgRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77EB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_SortRoomListByPosition, dll_base + 0x00037EB0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc* moo_ppRoomList{};
			D2DrlgRoomStrc* original_ppRoomList{};
			int nListSize{};

			const auto setup_data = [](
				D2DrlgRoomStrc*& ppRoomList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_ppRoomList);
			setup_data(original_ppRoomList);

			// Call both implementations
			sut(&moo_ppRoomList, nListSize);
			original(&original_ppRoomList, nListSize);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_ppRoomList, original_ppRoomList, "Comparing ppRoomList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD77F00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD77F00, dll_base + 0x00037F00);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom1{};
			D2DrlgRoomStrc moo_pDrlgRoom2{};
			D2DrlgRoomStrc original_pDrlgRoom1{};
			D2DrlgRoomStrc original_pDrlgRoom2{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			uint8_t nWarpId{};
			char nWarpFlag{};
			int nDirection{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom1,
				D2DrlgRoomStrc& pDrlgRoom2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom1, moo_pDrlgRoom2);
			setup_data(original_pDrlgRoom1, original_pDrlgRoom2);

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool, &moo_pDrlgRoom1, nWarpId, &moo_pDrlgRoom2, nWarpFlag, nDirection);
			const auto original_result = original(original_pMemPool, &original_pDrlgRoom1, nWarpId, &original_pDrlgRoom2, nWarpFlag, nDirection);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom1, original_pDrlgRoom1, "Comparing pDrlgRoom1");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom2, original_pDrlgRoom2, "Comparing pDrlgRoom2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD780E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_AllocPresetUnit, dll_base + 0x000380E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nUnitType{};
			int nIndex{};
			int nMode{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, moo_pMemPool, nUnitType, nIndex, nMode, nX, nY);
			const auto original_result = original(&original_pDrlgRoom, original_pMemPool, nUnitType, nIndex, nMode, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD78160" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_GetPresetUnits, dll_base + 0x00038160);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD78190" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_SetRoom, dll_base + 0x00038190);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2ActiveRoomStrc moo_pRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pRoom);
			setup_data(original_pDrlgRoom, original_pRoom);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pRoom);
			original(&original_pDrlgRoom, &original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD781A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_GetRGB_IntensityFromRoomEx, dll_base + 0x000381A0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			uint8_t moo_pIntensity{};
			uint8_t moo_pRed{};
			uint8_t moo_pGreen{};
			uint8_t moo_pBlue{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			uint8_t original_pIntensity{};
			uint8_t original_pRed{};
			uint8_t original_pGreen{};
			uint8_t original_pBlue{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom,
				uint8_t& pIntensity,
				uint8_t& pRed,
				uint8_t& pGreen,
				uint8_t& pBlue
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom, moo_pIntensity, moo_pRed, moo_pGreen, moo_pBlue);
			setup_data(original_pDrlgRoom, original_pIntensity, original_pRed, original_pGreen, original_pBlue);

			// Call both implementations
			sut(&moo_pDrlgRoom, &moo_pIntensity, &moo_pRed, &moo_pGreen, &moo_pBlue);
			original(&original_pDrlgRoom, &original_pIntensity, &original_pRed, &original_pGreen, &original_pBlue);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
			SKIP_MOO_CHECK_EQ(moo_pIntensity, original_pIntensity, "Comparing pIntensity");
			SKIP_MOO_CHECK_EQ(moo_pRed, original_pRed, "Comparing pRed");
			SKIP_MOO_CHECK_EQ(moo_pGreen, original_pGreen, "Comparing pGreen");
			SKIP_MOO_CHECK_EQ(moo_pBlue, original_pBlue, "Comparing pBlue");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD781E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_GetVisArrayFromLevelId, dll_base + 0x000381E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgStrc moo_pDrlg{};
			D2DrlgStrc original_pDrlg{};
			int nLevelId{};

			const auto setup_data = [](
				D2DrlgStrc& pDrlg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlg);
			setup_data(original_pDrlg);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlg, nLevelId);
			const auto original_result = original(&original_pDrlg, nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlg, original_pDrlg, "Comparing pDrlg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD78230" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGROOM_GetDrlgFromRoomEx, dll_base + 0x00038230);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pRoom{};
			D2DrlgRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom);
			const auto original_result = original(&original_pRoom);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
}
#endif
