#include <D2CommonTestDefines.h>

#ifdef CHAT_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Chat.h>


TEST_SUITE("D2ChatTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3BF0 (#10892)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CHAT_AllocHoverMsg, dll_base + 0x00083BF0);
		
		SUBCASE("")
		{
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			char szText{};
			int nTimeout{};

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool, &szText, nTimeout);
			const auto original_result = original(original_pMemPool, &szText, nTimeout);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3C80 (#10893)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CHAT_FreeHoverMsg, dll_base + 0x00083C80);
		
		SUBCASE("")
		{
			// Input data
			D2HoverTextStrc moo_pHoverMsg{};
			D2HoverTextStrc original_pHoverMsg{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2HoverTextStrc& pHoverMsg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHoverMsg);
			setup_data(original_pHoverMsg);

			// Call both implementations
			sut(moo_pMemPool, &moo_pHoverMsg);
			original(original_pMemPool, &original_pHoverMsg);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pHoverMsg, original_pHoverMsg, "Comparing pHoverMsg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3CA0 (#10894)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CHAT_GetDisplayTimeFromHoverMsg, dll_base + 0x00083CA0);
		
		SUBCASE("")
		{
			// Input data
			D2HoverTextStrc moo_pHoverMsg{};
			D2HoverTextStrc original_pHoverMsg{};

			const auto setup_data = [](
				D2HoverTextStrc& pHoverMsg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHoverMsg);
			setup_data(original_pHoverMsg);

			// Call both implementations
			const auto moo_result = sut(&moo_pHoverMsg);
			const auto original_result = original(&original_pHoverMsg);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHoverMsg, original_pHoverMsg, "Comparing pHoverMsg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3CB0 (#10895)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CHAT_GetTimeoutFromHoverMsg, dll_base + 0x00083CB0);
		
		SUBCASE("")
		{
			// Input data
			D2HoverTextStrc moo_pHoverMsg{};
			D2HoverTextStrc original_pHoverMsg{};

			const auto setup_data = [](
				D2HoverTextStrc& pHoverMsg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHoverMsg);
			setup_data(original_pHoverMsg);

			// Call both implementations
			const auto moo_result = sut(&moo_pHoverMsg);
			const auto original_result = original(&original_pHoverMsg);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHoverMsg, original_pHoverMsg, "Comparing pHoverMsg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3CC0 (#10896)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CHAT_CopyHoverMsgToBuffer, dll_base + 0x00083CC0);
		
		SUBCASE("")
		{
			// Input data
			D2HoverTextStrc moo_pHoverMsg{};
			char moo_szMessage{};
			D2HoverTextStrc original_pHoverMsg{};
			char original_szMessage{};

			const auto setup_data = [](
				D2HoverTextStrc& pHoverMsg,
				char& szMessage
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHoverMsg, moo_szMessage);
			setup_data(original_pHoverMsg, original_szMessage);

			// Call both implementations
			sut(&moo_pHoverMsg, &moo_szMessage);
			original(&original_pHoverMsg, &original_szMessage);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHoverMsg, original_pHoverMsg, "Comparing pHoverMsg");
			SKIP_MOO_CHECK_EQ(moo_szMessage, original_szMessage, "Comparing szMessage");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3CE0 (#10897)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CHAT_GetUsedFromHoverMsg, dll_base + 0x00083CE0);
		
		SUBCASE("")
		{
			// Input data
			D2HoverTextStrc moo_pHoverMsg{};
			D2HoverTextStrc original_pHoverMsg{};

			const auto setup_data = [](
				D2HoverTextStrc& pHoverMsg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHoverMsg);
			setup_data(original_pHoverMsg);

			// Call both implementations
			const auto moo_result = sut(&moo_pHoverMsg);
			const auto original_result = original(&original_pHoverMsg);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHoverMsg, original_pHoverMsg, "Comparing pHoverMsg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3CF0 (#10898)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CHAT_SetUsedInHoverMsg, dll_base + 0x00083CF0);
		
		SUBCASE("")
		{
			// Input data
			D2HoverTextStrc moo_pHoverMsg{};
			D2HoverTextStrc original_pHoverMsg{};
			BOOL bUsed{};

			const auto setup_data = [](
				D2HoverTextStrc& pHoverMsg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHoverMsg);
			setup_data(original_pHoverMsg);

			// Call both implementations
			sut(&moo_pHoverMsg, bUsed);
			original(&original_pHoverMsg, bUsed);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHoverMsg, original_pHoverMsg, "Comparing pHoverMsg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3D00 (#10899)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CHAT_GetLangIdFromHoverMsg, dll_base + 0x00083D00);
		
		SUBCASE("")
		{
			// Input data
			D2HoverTextStrc moo_pHoverMsg{};
			D2HoverTextStrc original_pHoverMsg{};

			const auto setup_data = [](
				D2HoverTextStrc& pHoverMsg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHoverMsg);
			setup_data(original_pHoverMsg);

			// Call both implementations
			const auto moo_result = sut(&moo_pHoverMsg);
			const auto original_result = original(&original_pHoverMsg);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHoverMsg, original_pHoverMsg, "Comparing pHoverMsg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3D10 (#10900)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CHAT_SetLangIdInHoverMsg, dll_base + 0x00083D10);
		
		SUBCASE("")
		{
			// Input data
			D2HoverTextStrc moo_pHoverMsg{};
			D2HoverTextStrc original_pHoverMsg{};
			uint8_t nLangId{};

			const auto setup_data = [](
				D2HoverTextStrc& pHoverMsg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHoverMsg);
			setup_data(original_pHoverMsg);

			// Call both implementations
			sut(&moo_pHoverMsg, nLangId);
			original(&original_pHoverMsg, nLangId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHoverMsg, original_pHoverMsg, "Comparing pHoverMsg");
		}
	}
}
#endif
