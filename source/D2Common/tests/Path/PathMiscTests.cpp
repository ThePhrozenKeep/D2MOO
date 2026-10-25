#include <D2CommonTestDefines.h>

#ifdef PATHMISC_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlg.h>
#include <Path/PathMisc.h>
#include <Units/Units.h>


TEST_SUITE("PathMiscTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA880" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAA880, dll_base + 0x0006A880);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			int moo_pTestDir{};
			int moo_pDirection{};
			D2PathInfoStrc original_pPathInfo{};
			int original_pTestDir{};
			int original_pDirection{};
			D2PathPointStrc pPoint{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo,
				int& pTestDir,
				int& pDirection
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo, moo_pTestDir, moo_pDirection);
			setup_data(original_pPathInfo, original_pTestDir, original_pDirection);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo, &moo_pTestDir, pPoint, &moo_pDirection);
			const auto original_result = original(&original_pPathInfo, &original_pTestDir, pPoint, &original_pDirection);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
			SKIP_MOO_CHECK_EQ(moo_pTestDir, original_pTestDir, "Comparing pTestDir");
			SKIP_MOO_CHECK_EQ(moo_pDirection, original_pDirection, "Comparing pDirection");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDABA50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDABA50, dll_base + 0x0006BA50);
		
		SUBCASE("")
		{
			D2PathPointStrc pPoint1{};
			D2PathPointStrc pPoint2{};

			// Call both implementations
			const auto moo_result = sut(pPoint1, pPoint2);
			const auto original_result = original(pPoint1, pPoint2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB6A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAB6A0, dll_base + 0x0006B6A0);
		
		SUBCASE("")
		{
			D2PathPointStrc pPoint1{};
			D2PathPointStrc pPoint2{};

			// Call both implementations
			const auto moo_result = sut(pPoint1, pPoint2);
			const auto original_result = original(pPoint1, pPoint2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB750" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAB750, dll_base + 0x0006B750);
		
		SUBCASE("")
		{
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};

			// Call both implementations
			const auto moo_result = sut(nX1, nY1, nX2, nY2);
			const auto original_result = original(nX1, nY1, nX2, nY2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB7D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAB7D0, dll_base + 0x0006B7D0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pTestDir{};
			int original_pTestDir{};
			int nUnused{};
			D2PathPointStrc pPoint1{};
			D2PathPointStrc pPoint2{};

			const auto setup_data = [](
				int& pTestDir
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTestDir);
			setup_data(original_pTestDir);

			// Call both implementations
			sut(&moo_pTestDir, nUnused, pPoint1, pPoint2);
			original(&original_pTestDir, nUnused, pPoint1, pPoint2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTestDir, original_pTestDir, "Comparing pTestDir");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB0B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAB0B0, dll_base + 0x0006B0B0);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo);
			setup_data(original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo);
			const auto original_result = original(&original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA9F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_Toward_6FDAA9F0, dll_base + 0x0006A9F0);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo);
			setup_data(original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo);
			const auto original_result = original(&original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAABF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_CheckCollisionsToNextPosition, dll_base + 0x0006ABF0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2PathPointStrc moo_pGameCoord{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2PathPointStrc original_pGameCoord{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2PathPointStrc& pGameCoord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pGameCoord);
			setup_data(original_pDynamicPath, original_pGameCoord);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_pGameCoord);
			const auto original_result = original(&original_pDynamicPath, &original_pGameCoord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pGameCoord, original_pGameCoord, "Comparing pGameCoord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB130" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_Straight_Compute, dll_base + 0x0006B130);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo);
			setup_data(original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo);
			const auto original_result = original(&original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB270" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_Knockback_Server, dll_base + 0x0006B270);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo);
			setup_data(original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo);
			const auto original_result = original(&original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB1E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_Leap_6FDAB1E0, dll_base + 0x0006B1E0);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo);
			setup_data(original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo);
			const auto original_result = original(&original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB240" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_Knockback_Client, dll_base + 0x0006B240);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo);
			setup_data(original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo);
			const auto original_result = original(&original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB0C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_BackupTurn_Compute, dll_base + 0x0006B0C0);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo);
			setup_data(original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo);
			const auto original_result = original(&original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB790" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetDirections_6FDAB790, dll_base + 0x0006B790);
		
		SUBCASE("")
		{
			// Input data
			int moo_pTestDir{};
			int original_pTestDir{};
			D2PathPointStrc pPoint1{};
			D2PathPointStrc pPoint2{};

			const auto setup_data = [](
				int& pTestDir
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTestDir);
			setup_data(original_pTestDir);

			// Call both implementations
			sut(&moo_pTestDir, pPoint1, pPoint2);
			original(&original_pTestDir, pPoint1, pPoint2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTestDir, original_pTestDir, "Comparing pTestDir");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB3C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ComputePathBlessedHammer_6FDAB3C0, dll_base + 0x0006B3C0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAAD10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_RayTrace, dll_base + 0x0006AD10);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2PathPointStrc moo_pPathDestination{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2PathPointStrc original_pPathDestination{};
			D2PathPointStrc tStartCoord{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2PathPointStrc& pPathDestination
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pPathDestination);
			setup_data(original_pDynamicPath, original_pPathDestination);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_pPathDestination, tStartCoord);
			const auto original_result = original(&original_pDynamicPath, &original_pPathDestination, tStartCoord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pPathDestination, original_pPathDestination, "Comparing pPathDestination");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB4A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ComputePathChargedBolt_6FDAB4A0, dll_base + 0x0006B4A0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2SeedStrc moo_pSeed{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2SeedStrc original_pSeed{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2SeedStrc& pSeed
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pSeed);
			setup_data(original_pDynamicPath, original_pSeed);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_pSeed);
			const auto original_result = original(&original_pDynamicPath, &original_pSeed);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pSeed, original_pSeed, "Comparing pSeed");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB610" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAB610, dll_base + 0x0006B610);
		
		SUBCASE("")
		{
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};

			// Call both implementations
			const auto moo_result = sut(nX1, nY1, nX2, nY2);
			const auto original_result = original(nX1, nY1, nX2, nY2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAC700 (#10215)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ComputeDirection, dll_base + 0x0006C700);
		
		SUBCASE("")
		{
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};

			// Call both implementations
			const auto moo_result = sut(nX1, nY1, nX2, nY2);
			const auto original_result = original(nX1, nY1, nX2, nY2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAC760" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ComputeDirectionFromPreciseCoords_6FDAC760, dll_base + 0x0006C760);
		
		SUBCASE("")
		{
			DWORD dwStartPrecisionX{};
			DWORD dwStartPrecisionY{};
			DWORD dwTargetPrecisionX{};
			DWORD dwTargetPrecisionY{};

			// Call both implementations
			const auto moo_result = sut(dwStartPrecisionX, dwStartPrecisionY, dwTargetPrecisionX, dwTargetPrecisionY);
			const auto original_result = original(dwStartPrecisionX, dwStartPrecisionY, dwTargetPrecisionX, dwTargetPrecisionY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAC790" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ComputeVelocityAndDirectionVectorsToNextPoint, dll_base + 0x0006C790);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pPath{};
			D2DynamicPathStrc original_pPath{};
			BOOL bNormalizeDirectionIfSamePos{};
			BOOL bForceDirectionNormalization{};

			const auto setup_data = [](
				D2DynamicPathStrc& pPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPath);
			setup_data(original_pPath);

			// Call both implementations
			sut(&moo_pPath, bNormalizeDirectionIfSamePos, bForceDirectionNormalization);
			original(&original_pPath, bNormalizeDirectionIfSamePos, bForceDirectionNormalization);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPath, original_pPath, "Comparing pPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAC8F0 (#10236)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10236, dll_base + 0x0006C8F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int a2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, a2);
			const auto original_result = original(&original_pUnit, a2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAC9A0 (#10226)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10226, dll_base + 0x0006C9A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			signed int a2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, a2);
			const auto original_result = original(&original_pUnit, a2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAD530 (#10227)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10227, dll_base + 0x0006D530);
		
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
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAD590 (#10229)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10229, dll_base + 0x0006D590);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2UnitStrc moo_pUnit{};
			D2ActiveRoomStrc moo_pDestRoom{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2UnitStrc original_pUnit{};
			D2ActiveRoomStrc original_pDestRoom{};
			uint32_t nDestX{};
			uint32_t nDestY{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2UnitStrc& pUnit,
				D2ActiveRoomStrc& pDestRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pUnit, moo_pDestRoom);
			setup_data(original_pDynamicPath, original_pUnit, original_pDestRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_pUnit, &moo_pDestRoom, nDestX, nDestY);
			const auto original_result = original(&original_pDynamicPath, &original_pUnit, &original_pDestRoom, nDestX, nDestY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pDestRoom, original_pDestRoom, "Comparing pDestRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDADA20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_RecacheRoom, dll_base + 0x0006DA20);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2ActiveRoomStrc moo_pHintRoom{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2ActiveRoomStrc original_pHintRoom{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2ActiveRoomStrc& pHintRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pHintRoom);
			setup_data(original_pDynamicPath, original_pHintRoom);

			// Call both implementations
			sut(&moo_pDynamicPath, &moo_pHintRoom);
			original(&original_pDynamicPath, &original_pHintRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pHintRoom, original_pHintRoom, "Comparing pHintRoom");
		}
	}
}
#endif
