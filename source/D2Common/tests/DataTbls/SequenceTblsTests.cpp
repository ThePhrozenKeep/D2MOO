#include <D2CommonTestDefines.h>

#ifdef SEQUENCE_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/SequenceTbls.h>
#include <Units/Units.h>


TEST_SUITE("SequenceTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD727A0 (#10682)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonSeqTxtRecordFromUnit, dll_base + 0x000327A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD727C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetSeqRecordFromUnit, dll_base + 0x000327C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD728A0 (#10683)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetSeqFramePointsCount, dll_base + 0x000328A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD728C0 (#10684)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetSeqFrameCount, dll_base + 0x000328C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD728E0 (#10685)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_ComputeSequenceAnimation, dll_base + 0x000328E0);
		
		SUBCASE("")
		{
			// Input data
			D2AnimSeqTxt moo_pMonSeqTxt{};
			unsigned int moo_pMode{};
			unsigned int moo_pFrame{};
			int moo_pDirection{};
			int moo_pEvent{};
			D2AnimSeqTxt original_pMonSeqTxt{};
			unsigned int original_pMode{};
			unsigned int original_pFrame{};
			int original_pDirection{};
			int original_pEvent{};
			int nTargetFramePoint{};
			int nCurrentFramePoint{};

			const auto setup_data = [](
				D2AnimSeqTxt& pMonSeqTxt,
				unsigned int& pMode,
				unsigned int& pFrame,
				int& pDirection,
				int& pEvent
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonSeqTxt, moo_pMode, moo_pFrame, moo_pDirection, moo_pEvent);
			setup_data(original_pMonSeqTxt, original_pMode, original_pFrame, original_pDirection, original_pEvent);

			// Call both implementations
			sut(&moo_pMonSeqTxt, nTargetFramePoint, nCurrentFramePoint, &moo_pMode, &moo_pFrame, &moo_pDirection, &moo_pEvent);
			original(&original_pMonSeqTxt, nTargetFramePoint, nCurrentFramePoint, &original_pMode, &original_pFrame, &original_pDirection, &original_pEvent);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonSeqTxt, original_pMonSeqTxt, "Comparing pMonSeqTxt");
			SKIP_MOO_CHECK_EQ(moo_pMode, original_pMode, "Comparing pMode");
			SKIP_MOO_CHECK_EQ(moo_pFrame, original_pFrame, "Comparing pFrame");
			SKIP_MOO_CHECK_EQ(moo_pDirection, original_pDirection, "Comparing pDirection");
			SKIP_MOO_CHECK_EQ(moo_pEvent, original_pEvent, "Comparing pEvent");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD72990 (#10686)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetSequenceEvent, dll_base + 0x00032990);
		
		SUBCASE("")
		{
			// Input data
			D2AnimSeqTxt moo_pMonSeqTxt{};
			int moo_pEvent{};
			D2AnimSeqTxt original_pMonSeqTxt{};
			int original_pEvent{};
			int nSeqFramePoint{};

			const auto setup_data = [](
				D2AnimSeqTxt& pMonSeqTxt,
				int& pEvent
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonSeqTxt, moo_pEvent);
			setup_data(original_pMonSeqTxt, original_pEvent);

			// Call both implementations
			sut(&moo_pMonSeqTxt, nSeqFramePoint, &moo_pEvent);
			original(&original_pMonSeqTxt, nSeqFramePoint, &original_pEvent);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonSeqTxt, original_pMonSeqTxt, "Comparing pMonSeqTxt");
			SKIP_MOO_CHECK_EQ(moo_pEvent, original_pEvent, "Comparing pEvent");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6F050" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonSeqTxt, dll_base + 0x0002F050);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6F200 (#11262)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonSeqTableRecord, dll_base + 0x0002F200);
		
		SUBCASE("")
		{
			int nSequence{};

			// Call both implementations
			const auto moo_result = sut(nSequence);
			const auto original_result = original(nSequence);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
}
#endif
