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


	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC36E0 (#10901)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_AllocTextHeader, dll_base + 0x000836E0);
		const auto [moo_free, original_free] = make_function_pair(TEXT_FreeTextHeader, dll_base + 0x00083710);

		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut(nullptr);
			const auto original_result = original(nullptr);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Free allocated memory
			moo_free(moo_result);
			original_free(original_result);
		}
	}

	// NOTE: Still needs to be confirmed manually since there's not really any observable behaviour
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3710 (#10902)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_FreeTextHeader, dll_base + 0x00083710);
		const auto [moo_alloc, original_alloc] = make_function_pair(TEXT_AllocTextHeader, dll_base + 0x000836E0);

		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc* moo_pTextHeader = moo_alloc(nullptr);
			D2TextHeaderStrc* original_pTextHeader = original_alloc(nullptr);

			// Call both implementations
			sut(moo_pTextHeader);
			original(original_pTextHeader);

			// Input data can not be compared since it was freed
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3760 (#10903)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);

		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			for (auto i = 0; i < 5; ++i)
			{
				short nStringId = random_unsigned_integer(0, 65535);
				int nMenu = random_unsigned_integer();

				// Call both implementations
				sut(&moo_pTextHeader, nStringId, nMenu);
				original(&original_pTextHeader, nStringId, nMenu);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC37A0 (#10904)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_RemoveNodeFromTextList, dll_base + 0x000837A0);
		const auto [moo_add_to_list, original_add_to_list] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);


		SUBCASE("")
		{
			// Input data
			const auto strings = random_unsigned_integer(1, 10);
			const auto string_ids = std::make_unique<short[]>(strings);

			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			for (auto i = 0; i < strings; ++i)
			{
				short nStringId = string_ids[i];
				int nMenu = random_unsigned_integer();

				// Call both implementations
				moo_add_to_list(&moo_pTextHeader, nStringId, nMenu);
				original_add_to_list(&original_pTextHeader, nStringId, nMenu);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}

			short nStringId = string_ids[random_unsigned_integer(1, strings)];

			// Call both implementations
			sut(&moo_pTextHeader, nStringId);
			original(&original_pTextHeader, nStringId);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC37F0 (#10905)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_GetMenuType2NodeCount, dll_base + 0x000837F0);
		const auto [moo_add_to_list, original_add_to_list] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);

		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			for (auto i = 0; i < 10; ++i)
			{
				short nStringId = i + 10000;
				int nMenu = random_unsigned_integer(0, 2);

				// Call both implementations
				moo_add_to_list(&moo_pTextHeader, nStringId, nMenu);
				original_add_to_list(&original_pTextHeader, nStringId, nMenu);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}

			// Call both implementations
			const auto moo_result = sut(&moo_pTextHeader);
			const auto original_result = original(&original_pTextHeader);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC33820 (#10906)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_GetStringIdOfMenuType2Node, dll_base + 0x00083820);
		const auto [moo_add_to_list, original_add_to_list] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);

		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			for (auto i = 0; i < 10; ++i)
			{
				short nStringId = i + 10000;
				int nMenu = random_unsigned_integer(0, 2);

				// Call both implementations
				moo_add_to_list(&moo_pTextHeader, nStringId, nMenu);
				original_add_to_list(&original_pTextHeader, nStringId, nMenu);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}

			for (auto i = 0; i < 10; ++i)
			{
				int nNodeId = i;

				// Call both implementations
				const auto moo_result = sut(&moo_pTextHeader, nNodeId);
				const auto original_result = original(&original_pTextHeader, nNodeId);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3850 (#10907)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_GetStringIdOfMenuType0Node, dll_base + 0x00083850);
		const auto [moo_add_to_list, original_add_to_list] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);

		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			for (auto i = 0; i < 10; ++i)
			{
				short nStringId = i + 10000;
				int nMenu = random_unsigned_integer(0, 2);

				// Call both implementations
				moo_add_to_list(&moo_pTextHeader, nStringId, nMenu);
				original_add_to_list(&original_pTextHeader, nStringId, nMenu);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}

			for (auto i = 0; i < 10; ++i)
			{
				int nNodeId = i;

				// Call both implementations
				const auto moo_result = sut(&moo_pTextHeader, nNodeId);
				const auto original_result = original(&original_pTextHeader, nNodeId);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3890 (#10908)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_GetStringIdOfMenuType1Node, dll_base + 0x00083890);
		const auto [moo_add_to_list, original_add_to_list] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);

		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			for (auto i = 0; i < 10; ++i)
			{
				short nStringId = i + 10000;
				int nMenu = random_unsigned_integer(0, 2);

				// Call both implementations
				moo_add_to_list(&moo_pTextHeader, nStringId, nMenu);
				original_add_to_list(&original_pTextHeader, nStringId, nMenu);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}

			for (auto i = 0; i < 10; ++i)
			{
				int nNodeId = i;

				// Call both implementations
				const auto moo_result = sut(&moo_pTextHeader, nNodeId);
				const auto original_result = original(&original_pTextHeader, nNodeId);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC38C0 (#10909)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_CreateMessageListFromTextHeader, dll_base + 0x000838C0);
		const auto [moo_add_to_list, original_add_to_list] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);

		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2MessageListStrc moo_pMsgList{};
			D2TextHeaderStrc original_pTextHeader{};
			D2MessageListStrc original_pMsgList{};

			for (auto i = 0; i < 8; ++i)
			{
				short nStringId = i + 10000;
				int nMenu = random_unsigned_integer(0, 2);

				// Call both implementations
				moo_add_to_list(&moo_pTextHeader, nStringId, nMenu);
				original_add_to_list(&original_pTextHeader, nStringId, nMenu);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}

			// Call both implementations
			sut(&moo_pTextHeader, &moo_pMsgList);
			original(&original_pTextHeader, &original_pMsgList);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			MOO_CHECK_EQ(moo_pMsgList, original_pMsgList, "Comparing pMsgList");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3970 (#10910)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_CreateTextHeaderFromMessageList, dll_base + 0x00083970);
		const auto [moo_add_to_list, original_add_to_list] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);

		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2MessageListStrc moo_pMsgList{};
			D2TextHeaderStrc original_pTextHeader{};
			D2MessageListStrc original_pMsgList{};

			for (auto i = 0; i < 8; ++i)
			{
				short nStringId = i + 10000;
				int nMenu = random_unsigned_integer(0, 2);

				// Call both implementations
				moo_add_to_list(&moo_pTextHeader, nStringId, nMenu);
				original_add_to_list(&original_pTextHeader, nStringId, nMenu);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}

			// Call both implementations
			sut(&moo_pTextHeader, &moo_pMsgList);
			original(&original_pTextHeader, &original_pMsgList);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			MOO_CHECK_EQ(moo_pMsgList, original_pMsgList, "Comparing pMsgList");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3A70 (#10911)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXT_SortTextNodeListByStringId, dll_base + 0x00083A70);
		const auto [moo_add_to_list, original_add_to_list] = make_function_pair(TEXT_AddNodeToTextList, dll_base + 0x00083760);

		SUBCASE("")
		{
			// Input data
			D2TextHeaderStrc moo_pTextHeader{};
			D2TextHeaderStrc original_pTextHeader{};

			for (auto i = 0; i < 8; ++i)
			{
				short nStringId = random_unsigned_integer(0, 100) + 10000;
				int nMenu = random_unsigned_integer(0, 2);

				// Call both implementations
				moo_add_to_list(&moo_pTextHeader, nStringId, nMenu);
				original_add_to_list(&original_pTextHeader, nStringId, nMenu);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
			}

			// Call both implementations
			sut(&moo_pTextHeader);
			original(&original_pTextHeader);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pTextHeader, original_pTextHeader, "Comparing pTextHeader");
		}
	}
}

#endif
