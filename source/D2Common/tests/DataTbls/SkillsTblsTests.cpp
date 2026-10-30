#include <D2CommonTestDefines.h>

#ifdef SKILLS_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/SkillsTbls.h>


TEST_SUITE("SkillsTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD498D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_MapSkillsTxtKeywordToNumber, dll_base + 0x000098D0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD49980" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD49980, dll_base + 0x00009980);
		
		SUBCASE("")
		{
			int nValue{};

			// Call both implementations
			const auto moo_result = sut(nValue);
			const auto original_result = original(nValue);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD49990" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD49990, dll_base + 0x00009990);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD49D20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_SkillCalcLinker, dll_base + 0x00009D20);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD49E40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_SkillDescCalcLinker, dll_base + 0x00009E40);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD49F60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadSkills_SkillDescTxt, dll_base + 0x00009F60);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD4E350" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadSkills_SkillDescTxt, dll_base + 0x0000E350);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
}
#endif
