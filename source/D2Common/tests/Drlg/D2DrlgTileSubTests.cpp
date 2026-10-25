#include <D2CommonTestDefines.h>

#ifdef DRLG_TILESUB_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgTileSub.h>


TEST_SUITE("D2DrlgTileSubTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A460" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGTILESUB_AddSecondaryBorder, dll_base + 0x0004A460);
		
		SUBCASE("")
		{
			// Input data
			D2UnkOutdoorStrc moo_a1{};
			D2UnkOutdoorStrc original_a1{};

			const auto setup_data = [](
				D2UnkOutdoorStrc& a1
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a1);
			setup_data(original_a1);

			// Call both implementations
			sut(&moo_a1);
			original(&original_a1);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a1, original_a1, "Comparing a1");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A750" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGTILESUB_TestReplaceSubPreset, dll_base + 0x0004A750);
		
		SUBCASE("")
		{
			// Input data
			D2UnkOutdoorStrc moo_a3{};
			D2DrlgSubstGroupStrc moo_pSubstGroup{};
			D2LvlSubTxt moo_pLvlSubTxtRecord{};
			D2UnkOutdoorStrc original_a3{};
			D2DrlgSubstGroupStrc original_pSubstGroup{};
			D2LvlSubTxt original_pLvlSubTxtRecord{};
			int a1{};
			int a2{};

			const auto setup_data = [](
				D2UnkOutdoorStrc& a3,
				D2DrlgSubstGroupStrc& pSubstGroup,
				D2LvlSubTxt& pLvlSubTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a3, moo_pSubstGroup, moo_pLvlSubTxtRecord);
			setup_data(original_a3, original_pSubstGroup, original_pLvlSubTxtRecord);

			// Call both implementations
			const auto moo_result = sut(a1, a2, &moo_a3, &moo_pSubstGroup, &moo_pLvlSubTxtRecord);
			const auto original_result = original(a1, a2, &original_a3, &original_pSubstGroup, &original_pLvlSubTxtRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
			SKIP_MOO_CHECK_EQ(moo_pSubstGroup, original_pSubstGroup, "Comparing pSubstGroup");
			SKIP_MOO_CHECK_EQ(moo_pLvlSubTxtRecord, original_pLvlSubTxtRecord, "Comparing pLvlSubTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8A8E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGTILESUB_ReplaceSubPreset, dll_base + 0x0004A8E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnkOutdoorStrc moo_a3{};
			D2DrlgSubstGroupStrc moo_pSubstGroup{};
			D2LvlSubTxt moo_pLvlSubTxtRecord{};
			D2UnkOutdoorStrc original_a3{};
			D2DrlgSubstGroupStrc original_pSubstGroup{};
			D2LvlSubTxt original_pLvlSubTxtRecord{};
			int a1{};
			int a2{};
			int a6{};

			const auto setup_data = [](
				D2UnkOutdoorStrc& a3,
				D2DrlgSubstGroupStrc& pSubstGroup,
				D2LvlSubTxt& pLvlSubTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a3, moo_pSubstGroup, moo_pLvlSubTxtRecord);
			setup_data(original_a3, original_pSubstGroup, original_pLvlSubTxtRecord);

			// Call both implementations
			sut(a1, a2, &moo_a3, &moo_pSubstGroup, &moo_pLvlSubTxtRecord, a6);
			original(a1, a2, &original_a3, &original_pSubstGroup, &original_pLvlSubTxtRecord, a6);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
			SKIP_MOO_CHECK_EQ(moo_pSubstGroup, original_pSubstGroup, "Comparing pSubstGroup");
			SKIP_MOO_CHECK_EQ(moo_pLvlSubTxtRecord, original_pLvlSubTxtRecord, "Comparing pLvlSubTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8AA80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD8AA80, dll_base + 0x0004AA80);
		
		SUBCASE("")
		{
			// Input data
			D2UnkOutdoorStrc2 moo_a1{};
			D2UnkOutdoorStrc2 original_a1{};

			const auto setup_data = [](
				D2UnkOutdoorStrc2& a1
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a1);
			setup_data(original_a1);

			// Call both implementations
			sut(&moo_a1);
			original(&original_a1);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a1, original_a1, "Comparing a1");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8ACE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD8ACE0, dll_base + 0x0004ACE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnkOutdoorStrc2 moo_a4{};
			D2DrlgSubstGroupStrc moo_pSubstGroup{};
			D2LvlSubTxt moo_pLvlSubTxtRecord{};
			D2UnkOutdoorStrc2 original_a4{};
			D2DrlgSubstGroupStrc original_pSubstGroup{};
			D2LvlSubTxt original_pLvlSubTxtRecord{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nX{};
			int nY{};
			int a7{};

			const auto setup_data = [](
				D2UnkOutdoorStrc2& a4,
				D2DrlgSubstGroupStrc& pSubstGroup,
				D2LvlSubTxt& pLvlSubTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a4, moo_pSubstGroup, moo_pLvlSubTxtRecord);
			setup_data(original_a4, original_pSubstGroup, original_pLvlSubTxtRecord);

			// Call both implementations
			sut(moo_pMemPool, nX, nY, &moo_a4, &moo_pSubstGroup, &moo_pLvlSubTxtRecord, a7);
			original(original_pMemPool, nX, nY, &original_a4, &original_pSubstGroup, &original_pLvlSubTxtRecord, a7);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_a4, original_a4, "Comparing a4");
			SKIP_MOO_CHECK_EQ(moo_pSubstGroup, original_pSubstGroup, "Comparing pSubstGroup");
			SKIP_MOO_CHECK_EQ(moo_pLvlSubTxtRecord, original_pLvlSubTxtRecord, "Comparing pLvlSubTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B010" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD8B010, dll_base + 0x0004B010);
		
		SUBCASE("")
		{
			// Input data
			D2UnkOutdoorStrc2 moo_a3{};
			D2DrlgSubstGroupStrc moo_pSubstGroup{};
			D2LvlSubTxt moo_pLvlSubTxtRecord{};
			D2UnkOutdoorStrc2 original_a3{};
			D2DrlgSubstGroupStrc original_pSubstGroup{};
			D2LvlSubTxt original_pLvlSubTxtRecord{};
			int a1{};
			int a2{};

			const auto setup_data = [](
				D2UnkOutdoorStrc2& a3,
				D2DrlgSubstGroupStrc& pSubstGroup,
				D2LvlSubTxt& pLvlSubTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a3, moo_pSubstGroup, moo_pLvlSubTxtRecord);
			setup_data(original_a3, original_pSubstGroup, original_pLvlSubTxtRecord);

			// Call both implementations
			const auto moo_result = sut(a1, a2, &moo_a3, &moo_pSubstGroup, &moo_pLvlSubTxtRecord);
			const auto original_result = original(a1, a2, &original_a3, &original_pSubstGroup, &original_pLvlSubTxtRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
			SKIP_MOO_CHECK_EQ(moo_pSubstGroup, original_pSubstGroup, "Comparing pSubstGroup");
			SKIP_MOO_CHECK_EQ(moo_pLvlSubTxtRecord, original_pLvlSubTxtRecord, "Comparing pLvlSubTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B130" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD8B130, dll_base + 0x0004B130);
		
		SUBCASE("")
		{
			// Input data
			D2UnkOutdoorStrc2 moo_a3{};
			D2DrlgSubstGroupStrc moo_pSubstGroup{};
			D2LvlSubTxt moo_pLvlSubTxtRecord{};
			D2UnkOutdoorStrc2 original_a3{};
			D2DrlgSubstGroupStrc original_pSubstGroup{};
			D2LvlSubTxt original_pLvlSubTxtRecord{};
			int a1{};
			int a2{};

			const auto setup_data = [](
				D2UnkOutdoorStrc2& a3,
				D2DrlgSubstGroupStrc& pSubstGroup,
				D2LvlSubTxt& pLvlSubTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a3, moo_pSubstGroup, moo_pLvlSubTxtRecord);
			setup_data(original_a3, original_pSubstGroup, original_pLvlSubTxtRecord);

			// Call both implementations
			const auto moo_result = sut(a1, a2, &moo_a3, &moo_pSubstGroup, &moo_pLvlSubTxtRecord);
			const auto original_result = original(a1, a2, &original_a3, &original_pSubstGroup, &original_pLvlSubTxtRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
			SKIP_MOO_CHECK_EQ(moo_pSubstGroup, original_pSubstGroup, "Comparing pSubstGroup");
			SKIP_MOO_CHECK_EQ(moo_pLvlSubTxtRecord, original_pLvlSubTxtRecord, "Comparing pLvlSubTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B290" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGTILESUB_DoSubstitutions, dll_base + 0x0004B290);
		
		SUBCASE("")
		{
			// Input data
			D2UnkOutdoorStrc2 moo_pOutdoorLevel{};
			D2LvlSubTxt moo_pLvlSubTxtRecord{};
			D2UnkOutdoorStrc2 original_pOutdoorLevel{};
			D2LvlSubTxt original_pLvlSubTxtRecord{};

			const auto setup_data = [](
				D2UnkOutdoorStrc2& pOutdoorLevel,
				D2LvlSubTxt& pLvlSubTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pOutdoorLevel, moo_pLvlSubTxtRecord);
			setup_data(original_pOutdoorLevel, original_pLvlSubTxtRecord);

			// Call both implementations
			sut(&moo_pOutdoorLevel, &moo_pLvlSubTxtRecord);
			original(&original_pOutdoorLevel, &original_pLvlSubTxtRecord);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pOutdoorLevel, original_pOutdoorLevel, "Comparing pOutdoorLevel");
			SKIP_MOO_CHECK_EQ(moo_pLvlSubTxtRecord, original_pLvlSubTxtRecord, "Comparing pLvlSubTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B640" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGTILESUB_InitializeDrlgFile, dll_base + 0x0004B640);
		
		SUBCASE("")
		{
			// Input data
			D2LvlSubTxt moo_pLvlSubTxtRecord{};
			D2LvlSubTxt original_pLvlSubTxtRecord{};
			HD2ARCHIVE hArchive{};

			const auto setup_data = [](
				D2LvlSubTxt& pLvlSubTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLvlSubTxtRecord);
			setup_data(original_pLvlSubTxtRecord);

			// Call both implementations
			sut(hArchive, &moo_pLvlSubTxtRecord);
			original(hArchive, &original_pLvlSubTxtRecord);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLvlSubTxtRecord, original_pLvlSubTxtRecord, "Comparing pLvlSubTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B770" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGTILESUB_FreeDrlgFile, dll_base + 0x0004B770);
		
		SUBCASE("")
		{
			// Input data
			D2LvlSubTxt moo_pLvlSubTxtRecord{};
			D2LvlSubTxt original_pLvlSubTxtRecord{};

			const auto setup_data = [](
				D2LvlSubTxt& pLvlSubTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLvlSubTxtRecord);
			setup_data(original_pLvlSubTxtRecord);

			// Call both implementations
			sut(&moo_pLvlSubTxtRecord);
			original(&original_pLvlSubTxtRecord);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLvlSubTxtRecord, original_pLvlSubTxtRecord, "Comparing pLvlSubTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8B7E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGTILESUB_PickSubThemes, dll_base + 0x0004B7E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgRoomStrc moo_pDrlgRoom{};
			D2DrlgRoomStrc original_pDrlgRoom{};
			int nSubType{};
			int nSubTheme{};

			const auto setup_data = [](
				D2DrlgRoomStrc& pDrlgRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgRoom);
			setup_data(original_pDrlgRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgRoom, nSubType, nSubTheme);
			const auto original_result = original(&original_pDrlgRoom, nSubType, nSubTheme);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoom, original_pDrlgRoom, "Comparing pDrlgRoom");
		}
	}
}
#endif
