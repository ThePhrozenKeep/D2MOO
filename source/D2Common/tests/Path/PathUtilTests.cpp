#include <D2CommonTestDefines.h>

#ifdef PATHUTIL_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Path/PathUtil.h>


TEST_SUITE("PathUtilTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB890" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATHUtil_AdvanceTowardsTarget_6FDAB890, dll_base + 0x0006B890);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_ptPath{};
			D2DynamicPathStrc original_ptPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& ptPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_ptPath);
			setup_data(original_ptPath);

			// Call both implementations
			sut(&moo_ptPath);
			original(&original_ptPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_ptPath, original_ptPath, "Comparing ptPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAB940" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAB940, dll_base + 0x0006B940);
		
		SUBCASE("")
		{
			// Input data
			D2PathPointStrc moo_pOutPathPoint{};
			D2DynamicPathStrc moo_ptPath{};
			D2PathPointStrc original_pOutPathPoint{};
			D2DynamicPathStrc original_ptPath{};

			const auto setup_data = [](
				D2PathPointStrc& pOutPathPoint,
				D2DynamicPathStrc& ptPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pOutPathPoint, moo_ptPath);
			setup_data(original_pOutPathPoint, original_ptPath);

			// Call both implementations
			sut(&moo_pOutPathPoint, &moo_ptPath);
			original(&original_pOutPathPoint, &original_ptPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pOutPathPoint, original_pOutPathPoint, "Comparing pOutPathPoint");
			SKIP_MOO_CHECK_EQ(moo_ptPath, original_ptPath, "Comparing ptPath");
		}
	}
}
#endif
