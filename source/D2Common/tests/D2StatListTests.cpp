#include <D2CommonTestDefines.h>

#ifdef STATLIST_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2StatList.h>
#include <GAME/Game.h>
#include <Units/Units.h>


TEST_SUITE("D2StatListTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB57C0 (#10563)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AreUnitsAligned, dll_base + 0x000757C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, &moo_pUnit2);
			const auto original_result = original(&original_pUnit1, &original_pUnit2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB5830" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDB5830, dll_base + 0x00075830);
		
		SUBCASE("")
		{
			// Input data
			D2StatListExStrc moo_pStatListEx{};
			D2StatListExStrc original_pStatListEx{};
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};

			const auto setup_data = [](
				D2StatListExStrc& pStatListEx
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatListEx);
			setup_data(original_pStatListEx);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatListEx, nLayer_StatId);
			const auto original_result = original(&original_pStatListEx, nLayer_StatId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatListEx, original_pStatListEx, "Comparing pStatListEx");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB6300" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FindStatIndex_6FDB6300, dll_base + 0x00076300);
		
		SUBCASE("")
		{
			// Input data
			D2StatsArrayStrc moo_pStatArray{};
			D2StatsArrayStrc original_pStatArray{};
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};

			const auto setup_data = [](
				D2StatsArrayStrc& pStatArray
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatArray);
			setup_data(original_pStatArray);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatArray, nLayer_StatId);
			const auto original_result = original(&original_pStatArray, nLayer_StatId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatArray, original_pStatArray, "Comparing pStatArray");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB6340" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetBaseStat_6FDB6340, dll_base + 0x00076340);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatListEx{};
			D2ItemStatCostTxt moo_pItemStatCostTxtRecord{};
			D2StatListStrc original_pStatListEx{};
			D2ItemStatCostTxt original_pItemStatCostTxtRecord{};
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};

			const auto setup_data = [](
				D2StatListStrc& pStatListEx,
				D2ItemStatCostTxt& pItemStatCostTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatListEx, moo_pItemStatCostTxtRecord);
			setup_data(original_pStatListEx, original_pItemStatCostTxtRecord);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatListEx, nLayer_StatId, &moo_pItemStatCostTxtRecord);
			const auto original_result = original(&original_pStatListEx, nLayer_StatId, &original_pItemStatCostTxtRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatListEx, original_pStatListEx, "Comparing pStatListEx");
			SKIP_MOO_CHECK_EQ(moo_pItemStatCostTxtRecord, original_pItemStatCostTxtRecord, "Comparing pItemStatCostTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB63E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetTotalStat_6FDB63E0, dll_base + 0x000763E0);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2ItemStatCostTxt moo_pItemStatCostTxtRecord{};
			D2StatListStrc original_pStatList{};
			D2ItemStatCostTxt original_pItemStatCostTxtRecord{};
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};

			const auto setup_data = [](
				D2StatListStrc& pStatList,
				D2ItemStatCostTxt& pItemStatCostTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList, moo_pItemStatCostTxtRecord);
			setup_data(original_pStatList, original_pItemStatCostTxtRecord);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList, nLayer_StatId, &moo_pItemStatCostTxtRecord);
			const auto original_result = original(&original_pStatList, nLayer_StatId, &original_pItemStatCostTxtRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
			SKIP_MOO_CHECK_EQ(moo_pItemStatCostTxtRecord, original_pItemStatCostTxtRecord, "Comparing pItemStatCostTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB64A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDB64A0, dll_base + 0x000764A0);
		
		SUBCASE("")
		{
			// Input data
			D2StatListExStrc moo_pStatListEx{};
			D2ItemStatCostTxt moo_pItemStatCostTxtRecord{};
			D2UnitStrc moo_pUnit{};
			D2StatListExStrc original_pStatListEx{};
			D2ItemStatCostTxt original_pItemStatCostTxtRecord{};
			D2UnitStrc original_pUnit{};
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};

			const auto setup_data = [](
				D2StatListExStrc& pStatListEx,
				D2ItemStatCostTxt& pItemStatCostTxtRecord,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatListEx, moo_pItemStatCostTxtRecord, moo_pUnit);
			setup_data(original_pStatListEx, original_pItemStatCostTxtRecord, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatListEx, nLayer_StatId, &moo_pItemStatCostTxtRecord, &moo_pUnit);
			const auto original_result = original(&original_pStatListEx, nLayer_StatId, &original_pItemStatCostTxtRecord, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatListEx, original_pStatListEx, "Comparing pStatListEx");
			SKIP_MOO_CHECK_EQ(moo_pItemStatCostTxtRecord, original_pItemStatCostTxtRecord, "Comparing pItemStatCostTxtRecord");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB6920" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FindStat_6FDB6920, dll_base + 0x00076920);
		
		SUBCASE("")
		{
			// Input data
			D2StatsArrayStrc moo_pStatArray{};
			D2StatsArrayStrc original_pStatArray{};
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};

			const auto setup_data = [](
				D2StatsArrayStrc& pStatArray
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatArray);
			setup_data(original_pStatArray);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatArray, nLayer_StatId);
			const auto original_result = original(&original_pStatArray, nLayer_StatId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatArray, original_pStatArray, "Comparing pStatArray");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB6970" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_InsertStatOrFail_6FDB6970, dll_base + 0x00076970);
		
		SUBCASE("")
		{
			// Input data
			D2StatsArrayStrc moo_pStatsArray{};
			D2StatsArrayStrc original_pStatsArray{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};

			const auto setup_data = [](
				D2StatsArrayStrc& pStatsArray
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatsArray);
			setup_data(original_pStatsArray);

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool, &moo_pStatsArray, nLayer_StatId);
			const auto original_result = original(original_pMemPool, &original_pStatsArray, nLayer_StatId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pStatsArray, original_pStatsArray, "Comparing pStatsArray");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB6A30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_RemoveStat_6FDB6A30, dll_base + 0x00076A30);
		
		SUBCASE("")
		{
			// Input data
			D2StatsArrayStrc moo_pStatEx{};
			D2StatStrc moo_pStat{};
			D2StatsArrayStrc original_pStatEx{};
			D2StatStrc original_pStat{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2StatsArrayStrc& pStatEx,
				D2StatStrc& pStat
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatEx, moo_pStat);
			setup_data(original_pStatEx, original_pStat);

			// Call both implementations
			sut(moo_pMemPool, &moo_pStatEx, &moo_pStat);
			original(original_pMemPool, &original_pStatEx, &original_pStat);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pStatEx, original_pStatEx, "Comparing pStatEx");
			SKIP_MOO_CHECK_EQ(moo_pStat, original_pStat, "Comparing pStat");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB6AB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_UpdateUnitStat_6FDB6AB0, dll_base + 0x00076AB0);
		
		SUBCASE("")
		{
			// Input data
			D2StatListExStrc moo_pStatList{};
			D2ItemStatCostTxt moo_pItemStatCostTxtRecord{};
			D2UnitStrc moo_pUnit{};
			D2StatListExStrc original_pStatList{};
			D2ItemStatCostTxt original_pItemStatCostTxtRecord{};
			D2UnitStrc original_pUnit{};
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};
			int nNewValue{};

			const auto setup_data = [](
				D2StatListExStrc& pStatList,
				D2ItemStatCostTxt& pItemStatCostTxtRecord,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList, moo_pItemStatCostTxtRecord, moo_pUnit);
			setup_data(original_pStatList, original_pItemStatCostTxtRecord, original_pUnit);

			// Call both implementations
			sut(&moo_pStatList, nLayer_StatId, nNewValue, &moo_pItemStatCostTxtRecord, &moo_pUnit);
			original(&original_pStatList, nLayer_StatId, nNewValue, &original_pItemStatCostTxtRecord, &original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
			SKIP_MOO_CHECK_EQ(moo_pItemStatCostTxtRecord, original_pItemStatCostTxtRecord, "Comparing pItemStatCostTxtRecord");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB6C10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDB6C10, dll_base + 0x00076C10);
		
		SUBCASE("")
		{
			// Input data
			D2StatListExStrc moo_pStatListEx{};
			D2UnitStrc moo_pUnit{};
			D2StatListExStrc original_pStatListEx{};
			D2UnitStrc original_pUnit{};
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};
			int nValue{};

			const auto setup_data = [](
				D2StatListExStrc& pStatListEx,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatListEx, moo_pUnit);
			setup_data(original_pStatListEx, original_pUnit);

			// Call both implementations
			sut(&moo_pStatListEx, nLayer_StatId, nValue, &moo_pUnit);
			original(&original_pStatListEx, nLayer_StatId, nValue, &original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatListEx, original_pStatListEx, "Comparing pStatListEx");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB6E30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_ExpireStatList_6FDB6E30, dll_base + 0x00076E30);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList);
			original(&original_pStatList);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7030 (#10485)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FreeStatList, dll_base + 0x00077030);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList);
			original(&original_pStatList);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7050" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_STATLIST_FreeStatListImpl_6FDB7050, dll_base + 0x00077050);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList);
			original(&original_pStatList);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7110 (#10527)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FreeStatListEx, dll_base + 0x00077110);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7140 (#10470)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AllocStatList, dll_base + 0x00077140);
		
		SUBCASE("")
		{
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			uint32_t fFilter{};
			uint32_t dwTimeout{};
			int nUnitType{};
			D2UnitGUID nUnitGUID{};

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool, fFilter, dwTimeout, nUnitType, nUnitGUID);
			const auto original_result = original(original_pMemPool, fFilter, dwTimeout, nUnitType, nUnitGUID);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7190 (#10526)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AllocStatListEx, dll_base + 0x00077190);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2GameStrc moo_pGame{};
			D2UnitStrc original_pUnit{};
			D2GameStrc original_pGame{};
			char nFlags{};
			StatListValueChangeFunc pfOnValueChanged{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2GameStrc& pGame
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pGame);
			setup_data(original_pUnit, original_pGame);

			// Call both implementations
			sut(&moo_pUnit, nFlags, pfOnValueChanged, &moo_pGame);
			original(&original_pUnit, nFlags, pfOnValueChanged, &original_pGame);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pGame, original_pGame, "Comparing pGame");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7260 (#10471)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetOwnerType, dll_base + 0x00077260);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList);
			const auto original_result = original(&original_pStatList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD912D0 (#10472)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetOwnerGUID, dll_base + 0x000512D0);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList);
			const auto original_result = original(&original_pStatList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7280 (#11304)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetBaseStatsCount, dll_base + 0x00077280);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList);
			const auto original_result = original(&original_pStatList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB72A0 (#11305)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetFullStatsCountFromUnit, dll_base + 0x000772A0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB72C0 (#10478)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetState, dll_base + 0x000772C0);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nState{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList, nState);
			original(&original_pStatList, nState);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB72E0 (#10479)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetState, dll_base + 0x000772E0);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList);
			const auto original_result = original(&original_pStatList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7300 (#10528)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetExpireFrame, dll_base + 0x00077300);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nExpireFrame{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList, nExpireFrame);
			original(&original_pStatList, nExpireFrame);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7320 (#10529)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetExpireFrame, dll_base + 0x00077320);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList);
			const auto original_result = original(&original_pStatList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7340 (#10475)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10475_PostStatToStatList, dll_base + 0x00077340);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2StatListStrc moo_pStatList{};
			D2UnitStrc original_pUnit{};
			D2StatListStrc original_pStatList{};
			BOOL bResetFlag{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pStatList);
			setup_data(original_pUnit, original_pStatList);

			// Call both implementations
			sut(&moo_pUnit, &moo_pStatList, bResetFlag);
			original(&original_pUnit, &original_pStatList, bResetFlag);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7560 (#10464)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AddStat, dll_base + 0x00077560);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nStatId{};
			int nValue{};
			uint16_t nLayer{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList, nStatId, nValue, nLayer);
			original(&original_pStatList, nStatId, nValue, nLayer);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7690" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_InsertStatModOrFail_6FDB7690, dll_base + 0x00077690);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			D2SLayerStatIdStrc::PackedType nLayer_StatId{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList, nLayer_StatId);
			original(&original_pStatList, nLayer_StatId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB77B0 (#10463)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetStat, dll_base + 0x000777B0);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nStatId{};
			int nValue{};
			uint16_t nLayer{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList, nStatId, nValue, nLayer);
			const auto original_result = original(&original_pStatList, nStatId, nValue, nLayer);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7910 (#10465)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetStatIfListIsValid, dll_base + 0x00077910);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nStatId{};
			int nValue{};
			uint16_t nLayer{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList, nStatId, nValue, nLayer);
			original(&original_pStatList, nStatId, nValue, nLayer);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7930 (#11294)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetBaseStat, dll_base + 0x00077930);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2UnitStrc moo_pUnit{};
			D2StatListStrc original_pStatList{};
			D2UnitStrc original_pUnit{};
			int nStatId{};
			int nValue{};
			uint16_t nLayer{};

			const auto setup_data = [](
				D2StatListStrc& pStatList,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList, moo_pUnit);
			setup_data(original_pStatList, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList, nStatId, nValue, nLayer, &moo_pUnit);
			const auto original_result = original(&original_pStatList, nStatId, nValue, nLayer, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7AB0 (#10517)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetUnitStat, dll_base + 0x00077AB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nStatId{};
			int nValue{};
			uint16_t nLayer{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nStatId, nValue, nLayer);
			original(&original_pUnit, nStatId, nValue, nLayer);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7B00 (#10518)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AddUnitStat, dll_base + 0x00077B00);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nStatId{};
			int nValue{};
			uint16_t nLayer{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nStatId, nValue, nLayer);
			original(&original_pUnit, nStatId, nValue, nLayer);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7C30 (#10519)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_UnitGetStatValue, dll_base + 0x00077C30);
		
		SUBCASE("")
		{
			D2UnitStrc pUnit{};
			int nStatId{};
			uint16_t nLayer{};

			// Call both implementations
			const auto moo_result = sut(&pUnit, nStatId, nLayer);
			const auto original_result = original(&pUnit, nStatId, nLayer);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7E30 (#10520)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_UnitGetItemStatOrSkillStatValue, dll_base + 0x00077E30);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nStatId{};
			uint16_t nLayer{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nStatId, nLayer);
			const auto original_result = original(&original_pUnit, nStatId, nLayer);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7B30 (#10521)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetUnitBaseStat, dll_base + 0x00077B30);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nStatId{};
			uint16_t nLayer{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nStatId, nLayer);
			const auto original_result = original(&original_pUnit, nStatId, nLayer);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB7F40 (#10522)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetUnitStatBonus, dll_base + 0x00077F40);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nStatId{};
			uint16_t nLayer{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nStatId, nLayer);
			const auto original_result = original(&original_pUnit, nStatId, nLayer);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB80C0 (#10515)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_DeactivateTemporaryStates, dll_base + 0x000780C0);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8120 (#10467)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10467, dll_base + 0x00078120);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nStat{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList, nStat);
			const auto original_result = original(&original_pStatList, nStat);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8150 (#10468)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_RemoveAllStats, dll_base + 0x00078150);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList);
			original(&original_pStatList);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8190" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_GetStateFromStatListEx_6FDB8190, dll_base + 0x00078190);
		
		SUBCASE("")
		{
			// Input data
			D2StatListExStrc moo_pStatListEx{};
			D2StatListExStrc original_pStatListEx{};
			int nStateId{};

			const auto setup_data = [](
				D2StatListExStrc& pStatListEx
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatListEx);
			setup_data(original_pStatListEx);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatListEx, nStateId);
			const auto original_result = original(&original_pStatListEx, nStateId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatListEx, original_pStatListEx, "Comparing pStatListEx");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB81E0 (#10480)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromUnitAndState, dll_base + 0x000781E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nState{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nState);
			const auto original_result = original(&original_pUnit, nState);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8200 (#10482)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromFlag, dll_base + 0x00078200);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nFlag{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList, nFlag);
			const auto original_result = original(&original_pStatList, nFlag);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8230 (#10481)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromUnitAndFlag, dll_base + 0x00078230);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nFlag{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nFlag);
			const auto original_result = original(&original_pUnit, nFlag);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8270 (#10483)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromUnitStateOrFlag, dll_base + 0x00078270);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nState{};
			int nFlag{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nState, nFlag);
			const auto original_result = original(&original_pUnit, nState, nFlag);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB82C0 (#10484)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromUnitStateAndFlag, dll_base + 0x000782C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nState{};
			int nFlag{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nState, nFlag);
			const auto original_result = original(&original_pUnit, nState, nFlag);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8310 (#10523)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_MergeStatLists, dll_base + 0x00078310);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pTarget{};
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pTarget{};
			D2UnitStrc original_pUnit{};
			BOOL bType{};

			const auto setup_data = [](
				D2UnitStrc& pTarget,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTarget, moo_pUnit);
			setup_data(original_pTarget, original_pUnit);

			// Call both implementations
			sut(&moo_pTarget, &moo_pUnit, bType);
			original(&original_pTarget, &original_pUnit, bType);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTarget, original_pTarget, "Comparing pTarget");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB83A0 (#10535)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetOwner, dll_base + 0x000783A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			BOOL moo_pStatNotDynamic{};
			D2UnitStrc original_pUnit{};
			BOOL original_pStatNotDynamic{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				BOOL& pStatNotDynamic
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pStatNotDynamic);
			setup_data(original_pUnit, original_pStatNotDynamic);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pStatNotDynamic);
			const auto original_result = original(&original_pUnit, &original_pStatNotDynamic);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pStatNotDynamic, original_pStatNotDynamic, "Comparing pStatNotDynamic");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8420 (#10512)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10512, dll_base + 0x00078420);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};
			int nStatId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			sut(&moo_pUnit1, &moo_pUnit2, nStatId, nullptr);
			original(&original_pUnit1, &original_pUnit2, nStatId, nullptr);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB84E0 (#10513)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10513, dll_base + 0x000784E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			sut(&moo_pUnit1, &moo_pUnit2, nullptr);
			original(&original_pUnit1, &original_pUnit2, nullptr);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB85D0 (#10511)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FreeModStats, dll_base + 0x000785D0);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8620 (#10562)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetUnitAlignment, dll_base + 0x00078620);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8750 (#10534)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10534, dll_base + 0x00078750);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8770 (#10530)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10530_D2CheckStatlistFlagDMGRed, dll_base + 0x00078770);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB87A0 (#10532)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetTotalStatValue_Layer0, dll_base + 0x000787A0);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatListEx{};
			D2StatListStrc original_pStatListEx{};
			int nStatId{};

			const auto setup_data = [](
				D2StatListStrc& pStatListEx
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatListEx);
			setup_data(original_pStatListEx);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatListEx, nStatId);
			const auto original_result = original(&original_pStatListEx, nStatId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatListEx, original_pStatListEx, "Comparing pStatListEx");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8890 (#10533)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_RemoveAllStatsFromOverlay, dll_base + 0x00078890);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8900" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_STATES_ToggleState_6FDB8900, dll_base + 0x00078900);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nState{};
			BOOL bSet{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nState, bSet);
			original(&original_pUnit, nState, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8A90" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_STATES_GetStatFlags_6FDB8A90, dll_base + 0x00078A90);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8AC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_STATES_GetListGfxFlags_6FDB8AC0, dll_base + 0x00078AC0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8BA0 (#11268)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetFullStatsDataFromUnit, dll_base + 0x00078BA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2StatStrc moo_pOutStatBuffer{};
			D2UnitStrc original_pUnit{};
			D2StatStrc original_pOutStatBuffer{};
			int nBufferSize{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2StatStrc& pOutStatBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pOutStatBuffer);
			setup_data(original_pUnit, original_pOutStatBuffer);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pOutStatBuffer, nBufferSize);
			const auto original_result = original(&original_pUnit, &original_pOutStatBuffer, nBufferSize);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pOutStatBuffer, original_pOutStatBuffer, "Comparing pOutStatBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8C00 (#11243)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetBaseStatsData, dll_base + 0x00078C00);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatListEx{};
			D2StatStrc moo_pOutStatBuffer{};
			D2StatListStrc original_pStatListEx{};
			D2StatStrc original_pOutStatBuffer{};
			int nBufferSize{};

			const auto setup_data = [](
				D2StatListStrc& pStatListEx,
				D2StatStrc& pOutStatBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatListEx, moo_pOutStatBuffer);
			setup_data(original_pStatListEx, original_pOutStatBuffer);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatListEx, &moo_pOutStatBuffer, nBufferSize);
			const auto original_result = original(&original_pStatListEx, &original_pOutStatBuffer, nBufferSize);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatListEx, original_pStatListEx, "Comparing pStatListEx");
			SKIP_MOO_CHECK_EQ(moo_pOutStatBuffer, original_pOutStatBuffer, "Comparing pOutStatBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8C50 (#10573)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_MergeBaseStats, dll_base + 0x00078C50);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pTargetStatList{};
			D2StatListStrc moo_pSourceStatlist{};
			D2StatListStrc original_pTargetStatList{};
			D2StatListStrc original_pSourceStatlist{};

			const auto setup_data = [](
				D2StatListStrc& pTargetStatList,
				D2StatListStrc& pSourceStatlist
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTargetStatList, moo_pSourceStatlist);
			setup_data(original_pTargetStatList, original_pSourceStatlist);

			// Call both implementations
			sut(&moo_pTargetStatList, &moo_pSourceStatlist);
			original(&original_pTargetStatList, &original_pSourceStatlist);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTargetStatList, original_pTargetStatList, "Comparing pTargetStatList");
			SKIP_MOO_CHECK_EQ(moo_pSourceStatlist, original_pSourceStatlist, "Comparing pSourceStatlist");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8CA0 (#10477)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetStatRemoveCallback, dll_base + 0x00078CA0);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			StatListRemoveCallback pfStatRemove{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList, pfStatRemove);
			original(&original_pStatList, pfStatRemove);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8CC0 (#10469)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10469, dll_base + 0x00078CC0);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8D30 (#10514)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_ClampStaminaManaHP, dll_base + 0x00078D30);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8EB0 (#10574)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10574, dll_base + 0x00078EB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nStateId{};
			BOOL bSet{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nStateId, bSet);
			const auto original_result = original(&original_pUnit, nStateId, bSet);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB8F30 (#10525)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10525, dll_base + 0x00078F30);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			sut(&moo_pUnit1, &moo_pUnit2);
			original(&original_pUnit1, &original_pUnit2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB91C0 (#10474)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10474, dll_base + 0x000791C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnused{};
			D2StatListStrc moo_pStatList{};
			D2UnitStrc original_pUnused{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnused,
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnused, moo_pStatList);
			setup_data(original_pUnused, original_pStatList);

			// Call both implementations
			sut(&moo_pUnused, &moo_pStatList);
			original(&original_pUnused, &original_pStatList);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnused, original_pUnused, "Comparing pUnused");
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB91D0 (#10564)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxLifeFromUnit, dll_base + 0x000791D0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB92C0 (#10565)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxManaFromUnit, dll_base + 0x000792C0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB93B0 (#10566)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxStaminaFromUnit, dll_base + 0x000793B0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB94A0 (#10567)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxDurabilityFromUnit, dll_base + 0x000794A0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB95D0 (#10568)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxDamageFromUnit, dll_base + 0x000795D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			BOOL b2Handed{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, b2Handed);
			const auto original_result = original(&original_pUnit, b2Handed);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB96F0 (#10569)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMinDamageFromUnit, dll_base + 0x000796F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			BOOL b2Handed{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, b2Handed);
			const auto original_result = original(&original_pUnit, b2Handed);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9810 (#10570)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxThrowDamageFromUnit, dll_base + 0x00079810);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9900 (#10571)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMinThrowDamageFromUnit, dll_base + 0x00079900);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB99F0 (#10572)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetDefenseFromUnit, dll_base + 0x000799F0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9AE0 (#10524)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_ExpireUnitStatlist, dll_base + 0x00079AE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnused{};
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnused{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnused,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnused, moo_pUnit);
			setup_data(original_pUnused, original_pUnit);

			// Call both implementations
			sut(&moo_pUnused, &moo_pUnit);
			original(&original_pUnused, &original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnused, original_pUnused, "Comparing pUnused");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9B00 (#10531)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10531_SetStatInStatListLayer0, dll_base + 0x00079B00);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nStatId{};
			int nValue{};
			int nUnused{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList, nStatId, nValue, nUnused);
			const auto original_result = original(&original_pStatList, nStatId, nValue, nUnused);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9B10 (#11248)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11248, dll_base + 0x00079B10);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnused{};
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnused{};
			D2UnitStrc original_pUnit{};
			int nStatId{};

			const auto setup_data = [](
				D2UnitStrc& pUnused,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnused, moo_pUnit);
			setup_data(original_pUnused, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnused, &moo_pUnit, nStatId);
			const auto original_result = original(&original_pUnused, &original_pUnit, nStatId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnused, original_pUnused, "Comparing pUnused");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E60 (#11264)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetSkillId, dll_base + 0x00069E60);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nSkillId{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList, nSkillId);
			original(&original_pStatList, nSkillId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9C10 (#11265)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetSkillId, dll_base + 0x00079C10);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList);
			const auto original_result = original(&original_pStatList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9C20 (#11266)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetSkillLevel, dll_base + 0x00079C20);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};
			int nSkillLevel{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			sut(&moo_pStatList, nSkillLevel);
			original(&original_pStatList, nSkillLevel);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E70 (#11267)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetSkillLevel, dll_base + 0x00069E70);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatList{};
			D2StatListStrc original_pStatList{};

			const auto setup_data = [](
				D2StatListStrc& pStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatList);
			setup_data(original_pStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatList);
			const auto original_result = original(&original_pStatList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9C30 (#11269)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11269_CopyStats, dll_base + 0x00079C30);
		
		SUBCASE("")
		{
			// Input data
			D2StatListExStrc moo_pStatListEx{};
			D2StatStrc moo_pBuffer{};
			D2StatListExStrc original_pStatListEx{};
			D2StatStrc original_pBuffer{};
			int nStatId{};
			int nBufferSize{};

			const auto setup_data = [](
				D2StatListExStrc& pStatListEx,
				D2StatStrc& pBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatListEx, moo_pBuffer);
			setup_data(original_pStatListEx, original_pBuffer);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatListEx, nStatId, &moo_pBuffer, nBufferSize);
			const auto original_result = original(&original_pStatListEx, nStatId, &original_pBuffer, nBufferSize);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatListEx, original_pStatListEx, "Comparing pStatListEx");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9C50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_CopyStats_6FDB9C50, dll_base + 0x00079C50);
		
		SUBCASE("")
		{
			// Input data
			D2StatListStrc moo_pStatListEx{};
			D2StatStrc moo_pBuffer{};
			D2StatListStrc original_pStatListEx{};
			D2StatStrc original_pBuffer{};
			int nStatId{};
			int nBufferSize{};

			const auto setup_data = [](
				D2StatListStrc& pStatListEx,
				D2StatStrc& pBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pStatListEx, moo_pBuffer);
			setup_data(original_pStatListEx, original_pBuffer);

			// Call both implementations
			const auto moo_result = sut(&moo_pStatListEx, nStatId, &moo_pBuffer, nBufferSize);
			const auto original_result = original(&original_pStatListEx, nStatId, &original_pBuffer, nBufferSize);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pStatListEx, original_pStatListEx, "Comparing pStatListEx");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9D20 (#11270)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_CopyStats, dll_base + 0x00079D20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2StatStrc moo_pBuffer{};
			D2UnitStrc original_pUnit{};
			D2StatStrc original_pBuffer{};
			int nStatId{};
			int nBufferSize{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2StatStrc& pBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pBuffer);
			setup_data(original_pUnit, original_pBuffer);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nStatId, &moo_pBuffer, nBufferSize);
			const auto original_result = original(&original_pUnit, nStatId, &original_pBuffer, nBufferSize);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9D60 (#11273)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11273, dll_base + 0x00079D60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nStatId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nStatId);
			const auto original_result = original(&original_pUnit, nStatId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9D90 (#11274)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11274, dll_base + 0x00079D90);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pTarget{};
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pTarget{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pTarget,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTarget, moo_pUnit);
			setup_data(original_pTarget, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pTarget, &moo_pUnit);
			const auto original_result = original(&original_pTarget, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTarget, original_pTarget, "Comparing pTarget");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9E60 (#11275)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11275, dll_base + 0x00079E60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pTarget{};
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pTarget{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pTarget,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTarget, moo_pUnit);
			setup_data(original_pTarget, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pTarget, &moo_pUnit);
			const auto original_result = original(&original_pTarget, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTarget, original_pTarget, "Comparing pTarget");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
}
#endif
