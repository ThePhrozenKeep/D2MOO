#include <D2CommonTestDefines.h>

#ifdef QUESTRECORD_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <cstring>
#include <memory>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2QuestRecord.h>


DYNAMIC_ARRAY_TYPE(uint8_t)


TEST_SUITE("D2QuestRecordTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE800 (#11107)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_GetQuestState, dll_base + 0x0006E800);

		SUBCASE("")
		{
			const auto size = sizeof(uint16_t) * NUM_QUEST_WORDS;
			const auto flags = std::make_unique<uint8_t[]>(size);

			for (auto j = 0; j < 16; ++j)
			{
				for (auto i = 0; i < NUM_QUEST_WORDS; ++i)
				{
					// Input data
					D2BitBufferStrc moo_pQuestRecord{};
					std::unique_ptr<uint8_t[]> moo_buffer;
					D2BitBufferStrc original_pQuestRecord{};
					std::unique_ptr<uint8_t[]> original_buffer;
					int nQuest = i;
					int nState = j;

					for (auto k = 0; k < size; ++k)
					{
						flags[k] = random_unsigned_integer(0, 255);
					}

					const auto setup_data = [&flags, size](
						D2BitBufferStrc& pQuestRecord,
						std::unique_ptr<uint8_t[]>& buffer
					) {
						buffer = std::make_unique<uint8_t[]>(size);
						std::memcpy(buffer.get(), flags.get(), size);
						BITMANIP_Initialize(&pQuestRecord, buffer.get(), size);
					};

					setup_data(moo_pQuestRecord, moo_buffer);
					setup_data(original_pQuestRecord, original_buffer);

					// Call both implementations
					const auto moo_result = sut(&moo_pQuestRecord, nQuest, nState);
					const auto original_result = original(&original_pQuestRecord, nQuest, nState);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					auto wrapped_moo_buffer = WrappedD2BitBufferStrc(moo_pQuestRecord, size);
					auto wrapped_original_buffer = WrappedD2BitBufferStrc(original_pQuestRecord, size);

					MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pQuestRecord");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE850 (#11108)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_SetQuestState, dll_base + 0x0006E850);

		SUBCASE("")
		{
			const auto size = sizeof(uint16_t) * NUM_QUEST_WORDS;

			for (auto j = 0; j < 16; ++j)
			{
				for (auto i = 0; i < NUM_QUEST_WORDS; ++i)
				{
					// Input data
					D2BitBufferStrc moo_pQuestRecord{};
					std::unique_ptr<uint8_t[]> moo_buffer;
					D2BitBufferStrc original_pQuestRecord{};
					std::unique_ptr<uint8_t[]> original_buffer;
					int nQuest = i;
					int nState = j;

					const auto setup_data = [size](
						D2BitBufferStrc& pQuestRecord,
						std::unique_ptr<uint8_t[]>& buffer
					) {
						buffer = std::make_unique<uint8_t[]>(size);
						std::memset(buffer.get(), 0, size);
						BITMANIP_Initialize(&pQuestRecord, buffer.get(), size);
					};

					setup_data(moo_pQuestRecord, moo_buffer);
					setup_data(original_pQuestRecord, original_buffer);

					// Call both implementations
					sut(&moo_pQuestRecord, nQuest, nState);
					original(&original_pQuestRecord, nQuest, nState);

					// Compare potentially modified input data
					auto wrapped_moo_buffer = WrappedD2BitBufferStrc(moo_pQuestRecord, size);
					auto wrapped_original_buffer = WrappedD2BitBufferStrc(original_pQuestRecord, size);

					MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pQuestRecord");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE890 (#11109)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_ClearQuestState, dll_base + 0x0006E890);

		SUBCASE("")
		{
			const auto size = sizeof(uint16_t) * NUM_QUEST_WORDS;
			const auto flags = std::make_unique<uint8_t[]>(size);

			for (auto j = 0; j < 16; ++j)
			{
				for (auto i = 0; i < NUM_QUEST_WORDS; ++i)
				{
					// Input data
					D2BitBufferStrc moo_pQuestRecord{};
					std::unique_ptr<uint8_t[]> moo_buffer;
					D2BitBufferStrc original_pQuestRecord{};
					std::unique_ptr<uint8_t[]> original_buffer;
					int nQuest = i;
					int nState = j;

					for (auto k = 0; k < size; ++k)
					{
						flags[k] = random_unsigned_integer(0, 255);
					}

					const auto setup_data = [&flags, size](
						D2BitBufferStrc& pQuestRecord,
						std::unique_ptr<uint8_t[]>& buffer
					) {
						buffer = std::make_unique<uint8_t[]>(size);
						std::memcpy(buffer.get(), flags.get(), size);
						BITMANIP_Initialize(&pQuestRecord, buffer.get(), size);
					};

					setup_data(moo_pQuestRecord, moo_buffer);
					setup_data(original_pQuestRecord, original_buffer);

					// Call both implementations
					sut(&moo_pQuestRecord, nQuest, nState);
					original(&original_pQuestRecord, nQuest, nState);

					// Compare potentially modified input data
					auto wrapped_moo_buffer = WrappedD2BitBufferStrc(moo_pQuestRecord, size);
					auto wrapped_original_buffer = WrappedD2BitBufferStrc(original_pQuestRecord, size);

					MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pQuestRecord");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE8D0 (#11110)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_ResetIntermediateStateFlags, dll_base + 0x0006E8D0);

		SUBCASE("")
		{
			const auto size = sizeof(uint16_t) * NUM_QUEST_WORDS;
			const auto flags = std::make_unique<uint8_t[]>(size);

			for (auto i = 0; i < NUM_QUEST_WORDS; ++i)
			{
				// Input data
				D2BitBufferStrc moo_pQuestRecord{};
				std::unique_ptr<uint8_t[]> moo_buffer;
				D2BitBufferStrc original_pQuestRecord{};
				std::unique_ptr<uint8_t[]> original_buffer;
				int nQuest = i;

				for (auto k = 0; k < size; ++k)
				{
					flags[k] = random_unsigned_integer(0, 255);
				}

				const auto setup_data = [&flags, size](
					D2BitBufferStrc& pQuestRecord,
					std::unique_ptr<uint8_t[]>& buffer
				) {
					buffer = std::make_unique<uint8_t[]>(size);
					std::memcpy(buffer.get(), flags.get(), size);
					BITMANIP_Initialize(&pQuestRecord, buffer.get(), size);
				};

				setup_data(moo_pQuestRecord, moo_buffer);
				setup_data(original_pQuestRecord, original_buffer);

				// Call both implementations
				sut(&moo_pQuestRecord, nQuest);
				original(&original_pQuestRecord, nQuest);

				// Compare potentially modified input data
				auto wrapped_moo_buffer = WrappedD2BitBufferStrc(moo_pQuestRecord, size);
				auto wrapped_original_buffer = WrappedD2BitBufferStrc(original_pQuestRecord, size);

				MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pQuestRecord");
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE920 (#11111)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_AllocRecord, dll_base + 0x0006E920);
		const auto [moo_free, original_free] = make_function_pair(QUESTRECORD_FreeRecord, dll_base + 0x0006E970);

		SUBCASE("")
		{
			const auto size = sizeof(uint16_t) * NUM_QUEST_WORDS;

			// Call both implementations
			const auto moo_result = sut(nullptr);
			const auto original_result = original(nullptr);

			// Compare return values
			auto wrapped_moo_buffer = WrappedD2BitBufferStrc(*moo_result, size);
			auto wrapped_original_buffer = WrappedD2BitBufferStrc(*original_result, size);

			MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pQuestRecord");

			// Free allocated memory
			moo_free(nullptr, moo_result);
			original_free(nullptr, original_result);
		}
	}

	// NOTE: Still needs to be confirmed manually since there's not really any observable behaviour
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE970 (#11112)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_FreeRecord, dll_base + 0x0006E970);
		const auto [moo_alloc, original_alloc] = make_function_pair(QUESTRECORD_AllocRecord, dll_base + 0x0006E920);

		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc* moo_pQuestRecord = moo_alloc(nullptr);
			D2BitBufferStrc* original_pQuestRecord = original_alloc(nullptr);

			// Call both implementations
			sut(nullptr, moo_pQuestRecord);
			original(nullptr, original_pQuestRecord);

			// Input data can not be compared since it was freed
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAE9B0 (#11113)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_CopyBufferToRecord, dll_base + 0x0006E9B0);

		SUBCASE("bResetStates = FALSE")
		{
			const auto size = sizeof(uint16_t) * NUM_QUEST_WORDS;
			const auto flags = std::make_unique<uint8_t[]>(size);

			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			std::unique_ptr<uint8_t[]> moo_buffer;
			D2BitBufferStrc original_pQuestRecord{};
			std::unique_ptr<uint8_t[]> original_buffer;
			uint16_t nBufferSize = size;
			BOOL bResetStates = FALSE;

			for (auto k = 0; k < size; ++k)
			{
				flags[k] = random_unsigned_integer(0, 255);
			}

			const auto setup_data = [&flags, size](
				D2BitBufferStrc& pQuestRecord,
				std::unique_ptr<uint8_t[]>& buffer
			) {
				buffer = std::make_unique<uint8_t[]>(size);
				std::memcpy(buffer.get(), flags.get(), size);
				BITMANIP_Initialize(&pQuestRecord, buffer.get(), size);
			};

			setup_data(moo_pQuestRecord, moo_buffer);
			setup_data(original_pQuestRecord, original_buffer);

			// Call both implementations
			sut(&moo_pQuestRecord, moo_buffer.get(), nBufferSize, bResetStates);
			original(&original_pQuestRecord, original_buffer.get(), nBufferSize, bResetStates);

			// Compare potentially modified input data
			auto wrapped_moo_buffer = WrappedD2BitBufferStrc(moo_pQuestRecord, size);
			auto wrapped_original_buffer = WrappedD2BitBufferStrc(original_pQuestRecord, size);

			MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pQuestRecord");
			auto wrapped_moo_buffer_array = DynamicArray<uint8_t>{ moo_buffer.get(), size };
			auto wrapped_original_buffer_array = DynamicArray<uint8_t>{ original_buffer.get(), size };
			MOO_CHECK_EQ(wrapped_moo_buffer_array, wrapped_original_buffer_array, "Comparing pBuffer");
		}

		SUBCASE("bResetStates = TRUE")
		{
			const auto size = sizeof(uint16_t) * NUM_QUEST_WORDS;
			const auto flags = std::make_unique<uint8_t[]>(size);

			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			std::unique_ptr<uint8_t[]> moo_buffer;
			D2BitBufferStrc original_pQuestRecord{};
			std::unique_ptr<uint8_t[]> original_buffer;
			uint16_t nBufferSize = size;
			BOOL bResetStates = TRUE;

			for (auto k = 0; k < size; ++k)
			{
				flags[k] = random_unsigned_integer(0, 255);
			}

			const auto setup_data = [&flags, size](
				D2BitBufferStrc& pQuestRecord,
				std::unique_ptr<uint8_t[]>& buffer
			) {
				buffer = std::make_unique<uint8_t[]>(size);
				std::memcpy(buffer.get(), flags.get(), size);
				BITMANIP_Initialize(&pQuestRecord, buffer.get(), size);
			};

			setup_data(moo_pQuestRecord, moo_buffer);
			setup_data(original_pQuestRecord, original_buffer);

			// Call both implementations
			sut(&moo_pQuestRecord, moo_buffer.get(), nBufferSize, bResetStates);
			original(&original_pQuestRecord, original_buffer.get(), nBufferSize, bResetStates);

			// Compare potentially modified input data
			auto wrapped_moo_buffer = WrappedD2BitBufferStrc(moo_pQuestRecord, size);
			auto wrapped_original_buffer = WrappedD2BitBufferStrc(original_pQuestRecord, size);

			MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pQuestRecord");
			auto wrapped_moo_buffer_array = DynamicArray<uint8_t>{ moo_buffer.get(), size };
			auto wrapped_original_buffer_array = DynamicArray<uint8_t>{ original_buffer.get(), size };
			MOO_CHECK_EQ(wrapped_moo_buffer_array, wrapped_original_buffer_array, "Comparing pBuffer");
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEA40 (#11114)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(QUESTRECORD_CopyRecordToBuffer, dll_base + 0x0006EA40);

		SUBCASE("")
		{
			const auto size = sizeof(uint16_t) * NUM_QUEST_WORDS;
			const auto flags = std::make_unique<uint8_t[]>(size);

			// Input data
			D2BitBufferStrc moo_pQuestRecord{};
			std::unique_ptr<uint8_t[]> moo_buffer;
			D2BitBufferStrc original_pQuestRecord{};
			std::unique_ptr<uint8_t[]> original_buffer;
			uint16_t nBufferSize = size;
			int nUnused{};

			for (auto k = 0; k < size; ++k)
			{
				flags[k] = random_unsigned_integer(0, 255);
			}

			const auto setup_data = [&flags, size](
				D2BitBufferStrc& pQuestRecord,
				std::unique_ptr<uint8_t[]>& buffer
			) {
				buffer = std::make_unique<uint8_t[]>(size);
				std::memcpy(buffer.get(), flags.get(), size);
				BITMANIP_Initialize(&pQuestRecord, buffer.get(), size);
			};

			setup_data(moo_pQuestRecord, moo_buffer);
			setup_data(original_pQuestRecord, original_buffer);

			// Call both implementations
			sut(&moo_pQuestRecord, moo_buffer.get(), nBufferSize, nUnused);
			original(&original_pQuestRecord, original_buffer.get(), nBufferSize, nUnused);

			// Compare potentially modified input data
			auto wrapped_moo_buffer = WrappedD2BitBufferStrc(moo_pQuestRecord, size);
			auto wrapped_original_buffer = WrappedD2BitBufferStrc(original_pQuestRecord, size);

			MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pQuestRecord");
			auto wrapped_moo_buffer_array = DynamicArray<uint8_t>{ moo_buffer.get(), size };
			auto wrapped_original_buffer_array = DynamicArray<uint8_t>{ original_buffer.get(), size };
			MOO_CHECK_EQ(wrapped_moo_buffer_array, wrapped_original_buffer_array, "Comparing pBuffer");
		}
	}
}

#endif
