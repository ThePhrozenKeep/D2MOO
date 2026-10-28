#include <D2CommonTestDefines.h>

#ifdef MISSILES_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/MissilesTbls.h>


TEST_SUITE("MissilesTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62EA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_MapMissilesTxtKeywordToNumber, dll_base + 0x00022EA0);
		
		SUBCASE("")
		{
			// Input data
			char moo_szKey{};
			char original_szKey{};

			const auto setup_data = [](
				char& szKey
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_szKey);
			setup_data(original_szKey);

			// Call both implementations
			const auto moo_result = sut(&moo_szKey);
			const auto original_result = original(&original_szKey);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_szKey, original_szKey, "Comparing szKey");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62F20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD62F20, dll_base + 0x00022F20);
		
		SUBCASE("")
		{
			// Input data
			char moo_szText{};
			int moo_a2{};
			char original_szText{};
			int original_a2{};
			int a3{};
			int nKeywordNumber{};

			const auto setup_data = [](
				char& szText,
				int& a2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_szText, moo_a2);
			setup_data(original_szText, original_a2);

			// Call both implementations
			const auto moo_result = sut(&moo_szText, &moo_a2, a3, nKeywordNumber);
			const auto original_result = original(&original_szText, &original_a2, a3, nKeywordNumber);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_szText, original_szText, "Comparing szText");
			SKIP_MOO_CHECK_EQ(moo_a2, original_a2, "Comparing a2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD630F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_MissileCalcLinker, dll_base + 0x000230F0);
		
		SUBCASE("")
		{
			// Input data
			char moo_pSrc{};
			char original_pSrc{};
			void* moo_pRecord = nullptr;
			void* original_pRecord = nullptr;
			int nOffset{};
			int nPosition{};
			int nTxtRow{};
			int nTxtColumn{};

			const auto setup_data = [](
				char& pSrc
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSrc);
			setup_data(original_pSrc);

			// Call both implementations
			sut(&moo_pSrc, moo_pRecord, nOffset, nPosition, nTxtRow, nTxtColumn);
			original(&original_pSrc, original_pRecord, nOffset, nPosition, nTxtRow, nTxtColumn);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSrc, original_pSrc, "Comparing pSrc");
			SKIP_MOO_CHECK_EQ(moo_pRecord, original_pRecord, "Comparing pRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD63180" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMissilesTxt, dll_base + 0x00023180);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD64B80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadMissilesTxt, dll_base + 0x00024B80);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD64BE0 (#10590)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMissileVelocityFromMissilesTxt, dll_base + 0x00024BE0);
		
		SUBCASE("")
		{
			int nMissileId{};
			int nLevel{};

			// Call both implementations
			const auto moo_result = sut(nMissileId, nLevel);
			const auto original_result = original(nMissileId, nLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
}
#endif
