#include <D2CommonTestDefines.h>

#ifdef TEXT_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Text.h>


TEST_SUITE("D2TextTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC36E0 (#10901)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_AllocTextHeader, dll_base + 0x000836E0);
		
		SUBCASE("")
		{
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool);
			const auto original_result = original(original_pMemPool);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3710 (#10902)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_FreeTextHeader, dll_base + 0x00083710);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader);
			setup_data(original_pTextHeader);

			// Call both implementations
			sut(&moo_pTextHeader);
			original(&original_pTextHeader);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3760 (#10903)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};
			short nStringId{};
			int nMenu{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader);
			setup_data(original_pTextHeader);

			// Call both implementations
			sut(&moo_pTextHeader, nStringId, nMenu);
			original(&original_pTextHeader, nStringId, nMenu);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC37A0 (#10904)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_RemoveNodeFromTextList, dll_base + 0x000837A0);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};
			short nStringId{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader);
			setup_data(original_pTextHeader);

			// Call both implementations
			sut(&moo_pTextHeader, nStringId);
			original(&original_pTextHeader, nStringId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC37F0 (#10905)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_GetMenuType2NodeCount, dll_base + 0x000837F0);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader);
			setup_data(original_pTextHeader);

			// Call both implementations
			const auto moo_result = sut(&moo_pTextHeader);
			const auto original_result = original(&original_pTextHeader);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC33820 (#10906)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_GetStringIdOfMenuType2Node, dll_base + 0x68DEF3820);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};
			int nNodeId{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader);
			setup_data(original_pTextHeader);

			// Call both implementations
			const auto moo_result = sut(&moo_pTextHeader, nNodeId);
			const auto original_result = original(&original_pTextHeader, nNodeId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3850 (#10907)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_GetStringIdOfMenuType1Or2Node, dll_base + 0x00083850);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};
			int nNodeId{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader);
			setup_data(original_pTextHeader);

			// Call both implementations
			const auto moo_result = sut(&moo_pTextHeader, nNodeId);
			const auto original_result = original(&original_pTextHeader, nNodeId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3890 (#10908)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_GetStringIdOfMenuType0Or2Node, dll_base + 0x00083890);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};
			int nNodeId{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader);
			setup_data(original_pTextHeader);

			// Call both implementations
			const auto moo_result = sut(&moo_pTextHeader, nNodeId);
			const auto original_result = original(&original_pTextHeader, nNodeId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC38C0 (#10909)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_CreateMessageListFromTextHeader, dll_base + 0x000838C0);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2MessageListStrc moo_pMsgList{};
			D2TextHeaderStrc original_pTextHeader{};
			D2MessageListStrc original_pMsgList{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader,
				D2MessageListStrc& pMsgList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader, moo_pMsgList);
			setup_data(original_pTextHeader, original_pMsgList);

			// Call both implementations
			sut(&moo_pTextHeader, &moo_pMsgList);
			original(&original_pTextHeader, &original_pMsgList);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			SKIP_MOO_CHECK_EQ(moo_pMsgList, original_pMsgList, "Comparing pMsgList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3970 (#10910)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_CreateTextHeaderFromMessageList, dll_base + 0x00083970);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2MessageListStrc moo_pMsgList{};
			D2TextHeaderStrc original_pTextHeader{};
			D2MessageListStrc original_pMsgList{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader,
				D2MessageListStrc& pMsgList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader, moo_pMsgList);
			setup_data(original_pTextHeader, original_pMsgList);

			// Call both implementations
			sut(&moo_pTextHeader, &moo_pMsgList);
			original(&original_pTextHeader, &original_pMsgList);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			SKIP_MOO_CHECK_EQ(moo_pMsgList, original_pMsgList, "Comparing pMsgList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3A70 (#10911)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_SortTextNodeListByStringId, dll_base + 0x00083A70);
		
		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			const auto setup_data = [](
				D2TextHeaderStrc& pTextHeader
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTextHeader);
			setup_data(original_pTextHeader);

			// Call both implementations
			sut(&moo_pTextHeader);
			original(&original_pTextHeader);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}
}
#endif
