#include <D2CommonTestDefines.h>

#ifdef HORADRIC_CUBE_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/HoradricCube.h>


TEST_SUITE("HoradricCubeTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD523E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CubeMainInputLinker, dll_base + 0x000123E0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52410" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CubeMainInputParser, dll_base + 0x00012410);
		
		SUBCASE("")
		{
			// Input data
			D2CubeInputItem moo_pCubeInput{};
			char moo_szInput{};
			D2CubeInputItem original_pCubeInput{};
			char original_szInput{};
			int nTxtRow{};
			int nItemId{};

			const auto setup_data = [](
				D2CubeInputItem& pCubeInput,
				char& szInput
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCubeInput, moo_szInput);
			setup_data(original_pCubeInput, original_szInput);

			// Call both implementations
			const auto moo_result = sut(&moo_pCubeInput, &moo_szInput, nTxtRow, nItemId);
			const auto original_result = original(&original_pCubeInput, &original_szInput, nTxtRow, nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCubeInput, original_pCubeInput, "Comparing pCubeInput");
			SKIP_MOO_CHECK_EQ(moo_szInput, original_szInput, "Comparing szInput");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD528D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CubeMainOutputLinker, dll_base + 0x000128D0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52910" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CubeMainOutputParser, dll_base + 0x00012910);
		
		SUBCASE("")
		{
			// Input data
			D2CubeOutputItem moo_pCubeOutputParam{};
			char moo_szOutput{};
			D2CubeOutputItem original_pCubeOutputParam{};
			char original_szOutput{};
			int nTxtRow{};
			int nItemId{};

			const auto setup_data = [](
				D2CubeOutputItem& pCubeOutputParam,
				char& szOutput
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCubeOutputParam, moo_szOutput);
			setup_data(original_pCubeOutputParam, original_szOutput);

			// Call both implementations
			const auto moo_result = sut(&moo_pCubeOutputParam, &moo_szOutput, nTxtRow, nItemId);
			const auto original_result = original(&original_pCubeOutputParam, &original_szOutput, nTxtRow, nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCubeOutputParam, original_pCubeOutputParam, "Comparing pCubeOutputParam");
			SKIP_MOO_CHECK_EQ(moo_szOutput, original_szOutput, "Comparing szOutput");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52FC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CubeMainParamLinker, dll_base + 0x00012FC0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD53030" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadCubeMainTxt, dll_base + 0x00013030);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD54250" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadCubeMainTxt, dll_base + 0x00014250);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD54260 (#11232)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetCubemainTxtRecord, dll_base + 0x00014260);
		
		SUBCASE("")
		{
			int nIndex{};

			// Call both implementations
			const auto moo_result = sut(nIndex);
			const auto original_result = original(nIndex);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD542C0 (#11233)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetCubemainTxtRecordCount, dll_base + 0x000142C0);
		
		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
}
#endif
