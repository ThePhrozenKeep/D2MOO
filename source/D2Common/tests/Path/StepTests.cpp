#include <D2CommonTestDefines.h>

#ifdef STEP_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlg.h>
#include <Path/Step.h>
#include <Units/Units.h>


TEST_SUITE("StepTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAC5E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetDirectionVector, dll_base + 0x0006C5E0);
		
		SUBCASE("")
		{
			// Input data
			D2CoordStrc moo_pDirectionVector{};
			int moo_pOutDirection{};
			D2CoordStrc original_pDirectionVector{};
			int original_pOutDirection{};
			DWORD dwStartPrecisionX{};
			DWORD dwStartPrecisionY{};
			DWORD dwTargetPrecisionX{};
			DWORD dwTargetPrecisionY{};

			const auto setup_data = [](
				D2CoordStrc& pDirectionVector,
				int& pOutDirection
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDirectionVector, moo_pOutDirection);
			setup_data(original_pDirectionVector, original_pOutDirection);

			// Call both implementations
			sut(&moo_pDirectionVector, &moo_pOutDirection, dwStartPrecisionX, dwStartPrecisionY, dwTargetPrecisionX, dwTargetPrecisionY);
			original(&original_pDirectionVector, &original_pOutDirection, dwStartPrecisionX, dwStartPrecisionY, dwTargetPrecisionX, dwTargetPrecisionY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDirectionVector, original_pDirectionVector, "Comparing pDirectionVector");
			SKIP_MOO_CHECK_EQ(moo_pOutDirection, original_pOutDirection, "Comparing pOutDirection");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDACEC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDACEC0, dll_base + 0x0006CEC0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2FP32_16 moo_a2{};
			D2UnitStrc* moo_pUnit{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2FP32_16 original_a2{};
			D2UnitStrc* original_pUnit{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2FP32_16& a2,
				D2UnitStrc*& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_a2, moo_pUnit);
			setup_data(original_pDynamicPath, original_a2, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_a2, &moo_pUnit);
			const auto original_result = original(&original_pDynamicPath, &original_a2, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_a2, original_a2, "Comparing a2");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAD5E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAD5E0, dll_base + 0x0006D5E0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2ActiveRoomStrc moo_pDestRoom{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2ActiveRoomStrc original_pDestRoom{};
			D2PathPointStrc tDest{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2ActiveRoomStrc& pDestRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pDestRoom);
			setup_data(original_pDynamicPath, original_pDestRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_pDestRoom, tDest);
			const auto original_result = original(&original_pDynamicPath, &original_pDestRoom, tDest);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pDestRoom, original_pDestRoom, "Comparing pDestRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE250" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATHMISC_SetRoom, dll_base + 0x0006E250);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pPath{};
			D2ActiveRoomStrc moo_pNewRoom{};
			D2DynamicPathStrc original_pPath{};
			D2ActiveRoomStrc original_pNewRoom{};

			const auto setup_data = [](
				D2DynamicPathStrc& pPath,
				D2ActiveRoomStrc& pNewRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPath, moo_pNewRoom);
			setup_data(original_pPath, original_pNewRoom);

			// Call both implementations
			sut(&moo_pPath, &moo_pNewRoom);
			original(&original_pPath, &original_pNewRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPath, original_pPath, "Comparing pPath");
			SKIP_MOO_CHECK_EQ(moo_pNewRoom, original_pNewRoom, "Comparing pNewRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDADF00 (#10230)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10230, dll_base + 0x0006DF00);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2ActiveRoomStrc moo_pRoom{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2ActiveRoomStrc original_pRoom{};
			int a2{};
			unsigned int a4{};
			__int16 a5{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pRoom);
			setup_data(original_pDynamicPath, original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, a2, &moo_pRoom, a4, a5);
			const auto original_result = original(&original_pDynamicPath, a2, &original_pRoom, a4, a5);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDADC20 (#10231)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10231, dll_base + 0x0006DC20);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2UnitStrc moo_pUnit_unused{};
			D2ActiveRoomStrc moo_pRooms{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2UnitStrc original_pUnit_unused{};
			D2ActiveRoomStrc original_pRooms{};
			uint16_t nX{};
			uint16_t nY{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2UnitStrc& pUnit_unused,
				D2ActiveRoomStrc& pRooms
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pUnit_unused, moo_pRooms);
			setup_data(original_pDynamicPath, original_pUnit_unused, original_pRooms);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_pUnit_unused, &moo_pRooms, nX, nY);
			const auto original_result = original(&original_pDynamicPath, &original_pUnit_unused, &original_pRooms, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pUnit_unused, original_pUnit_unused, "Comparing pUnit_unused");
			SKIP_MOO_CHECK_EQ(moo_pRooms, original_pRooms, "Comparing pRooms");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDADF50 (#10232)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10232, dll_base + 0x0006DF50);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pPath{};
			D2UnitStrc moo_pUnit{};
			D2ActiveRoomStrc moo_pDestRoom{};
			D2DynamicPathStrc original_pPath{};
			D2UnitStrc original_pUnit{};
			D2ActiveRoomStrc original_pDestRoom{};
			int nTargetX{};
			int nTargetY{};

			const auto setup_data = [](
				D2DynamicPathStrc& pPath,
				D2UnitStrc& pUnit,
				D2ActiveRoomStrc& pDestRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPath, moo_pUnit, moo_pDestRoom);
			setup_data(original_pPath, original_pUnit, original_pDestRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pPath, &moo_pUnit, &moo_pDestRoom, nTargetX, nTargetY);
			const auto original_result = original(&original_pPath, &original_pUnit, &original_pDestRoom, nTargetX, nTargetY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPath, original_pPath, "Comparing pPath");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pDestRoom, original_pDestRoom, "Comparing pDestRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE290 (#10233)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_RecacheRoomIfNeeded, dll_base + 0x0006E290);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath);
			original(&original_pDynamicPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE500 (#10234)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10234, dll_base + 0x0006E500);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE520 (#10235)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10235_PATH_UpdateRiderPath, dll_base + 0x0006E520);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pRiderUnit{};
			D2UnitStrc moo_pMountUnit{};
			D2UnitStrc original_pRiderUnit{};
			D2UnitStrc original_pMountUnit{};

			const auto setup_data = [](
				D2UnitStrc& pRiderUnit,
				D2UnitStrc& pMountUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRiderUnit, moo_pMountUnit);
			setup_data(original_pRiderUnit, original_pMountUnit);

			// Call both implementations
			sut(&moo_pRiderUnit, &moo_pMountUnit);
			original(&original_pRiderUnit, &original_pMountUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRiderUnit, original_pRiderUnit, "Comparing pRiderUnit");
			SKIP_MOO_CHECK_EQ(moo_pMountUnit, original_pMountUnit, "Comparing pMountUnit");
		}
	}
}
#endif
