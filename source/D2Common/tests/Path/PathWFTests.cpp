#include <D2CommonTestDefines.h>

#ifdef PATHWF_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Path/PathWF.h>


TEST_SUITE("PathWFTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDABAC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_FindSubpathWithoutObstacles, dll_base + 0x0006BAC0);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pInfo{};
			D2PathPointStrc moo_pPathPoints{};
			int moo_pSubPathStartIdx{};
			int moo_nMaxIndex{};
			D2PathInfoStrc original_pInfo{};
			D2PathPointStrc original_pPathPoints{};
			int original_pSubPathStartIdx{};
			int original_nMaxIndex{};
			D2PathPointStrc tSubPathStart{};
			int nMaxLength{};
			int nMajorDirection{};

			const auto setup_data = [](
				D2PathInfoStrc& pInfo,
				D2PathPointStrc& pPathPoints,
				int& pSubPathStartIdx,
				int& nMaxIndex
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInfo, moo_pPathPoints, moo_pSubPathStartIdx, moo_nMaxIndex);
			setup_data(original_pInfo, original_pPathPoints, original_pSubPathStartIdx, original_nMaxIndex);

			// Call both implementations
			const auto moo_result = sut(&moo_pInfo, tSubPathStart, &moo_pPathPoints, &moo_pSubPathStartIdx, &moo_nMaxIndex, nMaxLength, nMajorDirection);
			const auto original_result = original(&original_pInfo, tSubPathStart, &original_pPathPoints, &original_pSubPathStartIdx, &original_nMaxIndex, nMaxLength, nMajorDirection);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInfo, original_pInfo, "Comparing pInfo");
			SKIP_MOO_CHECK_EQ(moo_pPathPoints, original_pPathPoints, "Comparing pPathPoints");
			SKIP_MOO_CHECK_EQ(moo_pSubPathStartIdx, original_pSubPathStartIdx, "Comparing pSubPathStartIdx");
			SKIP_MOO_CHECK_EQ(moo_nMaxIndex, original_nMaxIndex, "Comparing nMaxIndex");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAC170" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SimplifyToLines, dll_base + 0x0006C170);
		
		SUBCASE("")
		{
			// Input data
			D2PathPointStrc moo_pOutPathPoints{};
			D2PathPointStrc moo_pInputPoints{};
			D2PathPointStrc original_pOutPathPoints{};
			D2PathPointStrc original_pInputPoints{};
			D2PathPointStrc tStartCoord{};
			signed int nbTempPoints{};

			const auto setup_data = [](
				D2PathPointStrc& pOutPathPoints,
				D2PathPointStrc& pInputPoints
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pOutPathPoints, moo_pInputPoints);
			setup_data(original_pOutPathPoints, original_pInputPoints);

			// Call both implementations
			const auto moo_result = sut(&moo_pOutPathPoints, &moo_pInputPoints, tStartCoord, nbTempPoints);
			const auto original_result = original(&original_pOutPathPoints, &original_pInputPoints, tStartCoord, nbTempPoints);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pOutPathPoints, original_pOutPathPoints, "Comparing pOutPathPoints");
			SKIP_MOO_CHECK_EQ(moo_pInputPoints, original_pInputPoints, "Comparing pInputPoints");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAC270" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ComputePathOrSlideAlongObstacles, dll_base + 0x0006C270);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_ptPathInfo{};
			D2PathInfoStrc original_ptPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& ptPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_ptPathInfo);
			setup_data(original_ptPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_ptPathInfo);
			const auto original_result = original(&original_ptPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_ptPathInfo, original_ptPathInfo, "Comparing ptPathInfo");
		}
	}
}
#endif
