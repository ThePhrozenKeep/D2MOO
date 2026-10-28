#include <D2CommonTestDefines.h>

#ifdef QUESTRECORD_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2QuestRecord.h>


TEST_SUITE("D2QuestRecordTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE800 (#11107)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_GetQuestState, dll_base + 0x0006E800);
		
		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			D2BitBufferStrc original_pQuestRecord{};
			int nQuest{};
			int nState{};

			const auto setup_data = [](
				D2BitBufferStrc& pQuestRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pQuestRecord);
			setup_data(original_pQuestRecord);

			// Call both implementations
			const auto moo_result = sut(&moo_pQuestRecord, nQuest, nState);
			const auto original_result = original(&original_pQuestRecord, nQuest, nState);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pQuestRecord, original_pQuestRecord, "Comparing pQuestRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE850 (#11108)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_SetQuestState, dll_base + 0x0006E850);
		
		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			D2BitBufferStrc original_pQuestRecord{};
			int nQuest{};
			int nState{};

			const auto setup_data = [](
				D2BitBufferStrc& pQuestRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pQuestRecord);
			setup_data(original_pQuestRecord);

			// Call both implementations
			sut(&moo_pQuestRecord, nQuest, nState);
			original(&original_pQuestRecord, nQuest, nState);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pQuestRecord, original_pQuestRecord, "Comparing pQuestRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE890 (#11109)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_ClearQuestState, dll_base + 0x0006E890);
		
		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			D2BitBufferStrc original_pQuestRecord{};
			int nQuest{};
			int nState{};

			const auto setup_data = [](
				D2BitBufferStrc& pQuestRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pQuestRecord);
			setup_data(original_pQuestRecord);

			// Call both implementations
			sut(&moo_pQuestRecord, nQuest, nState);
			original(&original_pQuestRecord, nQuest, nState);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pQuestRecord, original_pQuestRecord, "Comparing pQuestRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE8D0 (#11110)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_ResetIntermediateStateFlags, dll_base + 0x0006E8D0);
		
		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			D2BitBufferStrc original_pQuestRecord{};
			int nQuest{};

			const auto setup_data = [](
				D2BitBufferStrc& pQuestRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pQuestRecord);
			setup_data(original_pQuestRecord);

			// Call both implementations
			sut(&moo_pQuestRecord, nQuest);
			original(&original_pQuestRecord, nQuest);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pQuestRecord, original_pQuestRecord, "Comparing pQuestRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE920 (#11111)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_AllocRecord, dll_base + 0x0006E920);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE970 (#11112)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_FreeRecord, dll_base + 0x0006E970);
		
		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			D2BitBufferStrc original_pQuestRecord{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2BitBufferStrc& pQuestRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pQuestRecord);
			setup_data(original_pQuestRecord);

			// Call both implementations
			sut(moo_pMemPool, &moo_pQuestRecord);
			original(original_pMemPool, &original_pQuestRecord);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pQuestRecord, original_pQuestRecord, "Comparing pQuestRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE9B0 (#11113)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_CopyBufferToRecord, dll_base + 0x0006E9B0);
		
		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			uint8_t moo_pBuffer{};
			D2BitBufferStrc original_pQuestRecord{};
			uint8_t original_pBuffer{};
			uint16_t nBufferSize{};
			BOOL bResetStates{};

			const auto setup_data = [](
				D2BitBufferStrc& pQuestRecord,
				uint8_t& pBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pQuestRecord, moo_pBuffer);
			setup_data(original_pQuestRecord, original_pBuffer);

			// Call both implementations
			sut(&moo_pQuestRecord, &moo_pBuffer, nBufferSize, bResetStates);
			original(&original_pQuestRecord, &original_pBuffer, nBufferSize, bResetStates);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pQuestRecord, original_pQuestRecord, "Comparing pQuestRecord");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEA40 (#11114)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_CopyRecordToBuffer, dll_base + 0x0006EA40);
		
		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			uint8_t moo_pBuffer{};
			D2BitBufferStrc original_pQuestRecord{};
			uint8_t original_pBuffer{};
			uint16_t nBufferSize{};
			int nUnused{};

			const auto setup_data = [](
				D2BitBufferStrc& pQuestRecord,
				uint8_t& pBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pQuestRecord, moo_pBuffer);
			setup_data(original_pQuestRecord, original_pBuffer);

			// Call both implementations
			sut(&moo_pQuestRecord, &moo_pBuffer, nBufferSize, nUnused);
			original(&original_pQuestRecord, &original_pBuffer, nBufferSize, nUnused);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pQuestRecord, original_pQuestRecord, "Comparing pQuestRecord");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
		}
	}
}
#endif
