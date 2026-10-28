#include <D2CommonTestDefines.h>

#ifdef IDASTAR_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Path/IDAStar.h>


TEST_SUITE("IDAStarTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA7970" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_IdaStar_6FDA7970, dll_base + 0x00067970);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA7D40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_IDAStar_VisitNodes, dll_base + 0x00067D40);
		
		SUBCASE("")
		{
			// Input data
			D2PathIDAStarContextStrc moo_pContext{};
			D2PathInfoStrc moo_pPathInfo{};
			D2PathIDAStarContextStrc original_pContext{};
			D2PathInfoStrc original_pPathInfo{};
			int nFScoreCutoff{};

			const auto setup_data = [](
				D2PathIDAStarContextStrc& pContext,
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pContext, moo_pPathInfo);
			setup_data(original_pContext, original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pContext, nFScoreCutoff, &moo_pPathInfo);
			const auto original_result = original(&original_pContext, nFScoreCutoff, &original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pContext, original_pContext, "Comparing pContext");
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
}
#endif
