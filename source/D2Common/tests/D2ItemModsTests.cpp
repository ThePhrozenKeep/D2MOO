#include <D2CommonTestDefines.h>

#ifdef ITEMMODS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2ItemMods.h>
#include <D2StatList.h>
#include <Units/Units.h>


TEST_SUITE("D2ItemModsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92640 (#10844)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10844_ITEMMODS_First, dll_base + 0x00052640);
		
		SUBCASE("")
		{
			// Input data
			int moo_pLayer{};
			int moo_pValue{};
			int original_pLayer{};
			int original_pValue{};
			int nDataBits{};

			const auto setup_data = [](
				int& pLayer,
				int& pValue
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLayer, moo_pValue);
			setup_data(original_pLayer, original_pValue);

			// Call both implementations
			sut(nDataBits, &moo_pLayer, &moo_pValue);
			original(nDataBits, &original_pLayer, &original_pValue);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLayer, original_pLayer, "Comparing pLayer");
			SKIP_MOO_CHECK_EQ(moo_pValue, original_pValue, "Comparing pValue");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92670 (#10846)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10846, dll_base + 0x00052670);
		
		SUBCASE("")
		{
			// Input data
			int moo_a2{};
			int moo_a3{};
			int moo_a4{};
			int moo_a5{};
			int original_a2{};
			int original_a3{};
			int original_a4{};
			int original_a5{};
			int nDataBits{};

			const auto setup_data = [](
				int& a2,
				int& a3,
				int& a4,
				int& a5
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a2, moo_a3, moo_a4, moo_a5);
			setup_data(original_a2, original_a3, original_a4, original_a5);

			// Call both implementations
			sut(nDataBits, &moo_a2, &moo_a3, &moo_a4, &moo_a5);
			original(nDataBits, &original_a2, &original_a3, &original_a4, &original_a5);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a2, original_a2, "Comparing a2");
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
			SKIP_MOO_CHECK_EQ(moo_a4, original_a4, "Comparing a4");
			SKIP_MOO_CHECK_EQ(moo_a5, original_a5, "Comparing a5");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD926C0 (#11293)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_GetItemCharges, dll_base + 0x000526C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			int moo_pValue{};
			D2StatListStrc* moo_ppStatList{};
			D2UnitStrc original_pItem{};
			int original_pValue{};
			D2StatListStrc* original_ppStatList{};
			int nSkillId{};
			int nSkillLevel{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				int& pValue,
				D2StatListStrc*& ppStatList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pValue, moo_ppStatList);
			setup_data(original_pItem, original_pValue, original_ppStatList);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, nSkillId, nSkillLevel, &moo_pValue, &moo_ppStatList);
			const auto original_result = original(&original_pItem, nSkillId, nSkillLevel, &original_pValue, &original_ppStatList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pValue, original_pValue, "Comparing pValue");
			SKIP_MOO_CHECK_EQ(moo_ppStatList, original_ppStatList, "Comparing ppStatList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD927D0 (#10847)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_UpdateItemWithSkillCharges, dll_base + 0x000527D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nSkillId{};
			int nSkillLevel{};
			int a4{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, nSkillId, nSkillLevel, a4);
			const auto original_result = original(&original_pItem, nSkillId, nSkillLevel, a4);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD928D0 (#10843)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_GetByTimeAdjustment, dll_base + 0x000528D0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pItemModPeriodOfDay{};
			int moo_pItemModMin{};
			int moo_pItemModMax{};
			int original_pItemModPeriodOfDay{};
			int original_pItemModMin{};
			int original_pItemModMax{};
			int nAmount{};
			int nPeriodOfDay{};
			int nBaseTime{};

			const auto setup_data = [](
				int& pItemModPeriodOfDay,
				int& pItemModMin,
				int& pItemModMax
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItemModPeriodOfDay, moo_pItemModMin, moo_pItemModMax);
			setup_data(original_pItemModPeriodOfDay, original_pItemModMin, original_pItemModMax);

			// Call both implementations
			const auto moo_result = sut(nAmount, nPeriodOfDay, nBaseTime, &moo_pItemModPeriodOfDay, &moo_pItemModMin, &moo_pItemModMax);
			const auto original_result = original(nAmount, nPeriodOfDay, nBaseTime, &original_pItemModPeriodOfDay, &original_pItemModMin, &original_pItemModMax);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItemModPeriodOfDay, original_pItemModPeriodOfDay, "Comparing pItemModPeriodOfDay");
			SKIP_MOO_CHECK_EQ(moo_pItemModMin, original_pItemModMin, "Comparing pItemModMin");
			SKIP_MOO_CHECK_EQ(moo_pItemModMax, original_pItemModMax, "Comparing pItemModMax");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD929A0 (#10849)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10849, dll_base + 0x000529A0);
		
		SUBCASE("")
		{
			int a1{};
			int a2{};

			// Call both implementations
			const auto moo_result = sut(a1, a2);
			const auto original_result = original(a1, a2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD929B0 (#10845)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10845, dll_base + 0x000529B0);
		
		SUBCASE("")
		{
			// Input data
			int moo_a2{};
			int moo_a3{};
			int moo_a4{};
			int original_a2{};
			int original_a3{};
			int original_a4{};
			int nDataBits{};

			const auto setup_data = [](
				int& a2,
				int& a3,
				int& a4
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a2, moo_a3, moo_a4);
			setup_data(original_a2, original_a3, original_a4);

			// Call both implementations
			sut(nDataBits, &moo_a2, &moo_a3, &moo_a4);
			original(nDataBits, &original_a2, &original_a3, &original_a4);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a2, original_a2, "Comparing a2");
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
			SKIP_MOO_CHECK_EQ(moo_a4, original_a4, "Comparing a4");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD929E0 (#10850)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10850, dll_base + 0x000529E0);
		
		SUBCASE("")
		{
			int a1{};
			int a2{};
			int a3{};

			// Call both implementations
			const auto moo_result = sut(a1, a2, a3);
			const auto original_result = original(a1, a2, a3);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92A00 (#10848)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10848, dll_base + 0x00052A00);
		
		SUBCASE("")
		{
			// Input data
			int moo_pClass{};
			int moo_pTab{};
			int moo_pLevel{};
			int original_pClass{};
			int original_pTab{};
			int original_pLevel{};
			int nDataBits{};

			const auto setup_data = [](
				int& pClass,
				int& pTab,
				int& pLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pClass, moo_pTab, moo_pLevel);
			setup_data(original_pClass, original_pTab, original_pLevel);

			// Call both implementations
			sut(nDataBits, &moo_pClass, &moo_pTab, &moo_pLevel);
			original(nDataBits, &original_pClass, &original_pTab, &original_pLevel);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pClass, original_pClass, "Comparing pClass");
			SKIP_MOO_CHECK_EQ(moo_pTab, original_pTab, "Comparing pTab");
			SKIP_MOO_CHECK_EQ(moo_pLevel, original_pLevel, "Comparing pLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92A60 (#10851)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10851, dll_base + 0x00052A60);
		
		SUBCASE("")
		{
			int a1{};
			int a2{};
			int a3{};

			// Call both implementations
			const auto moo_result = sut(a1, a2, a3);
			const auto original_result = original(a1, a2, a3);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92A80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD92A80, dll_base + 0x00052A80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92C40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_GetOrCreateStatList, dll_base + 0x00052C40);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			int nState{};
			int fFilter{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem);
			setup_data(original_pUnit, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pItem, nState, fFilter);
			const auto original_result = original(&original_pUnit, &original_pItem, nState, fFilter);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92CF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD92CF0, dll_base + 0x00052CF0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nStatId{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nStatId);
			original(&original_pItem, nStatId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92E80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD92E80, dll_base + 0x00052E80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92EB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD92EB0, dll_base + 0x00052EB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int a7{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, a7, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, a7, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD93170" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD93170, dll_base + 0x00053170);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD931C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD931C0, dll_base + 0x000531C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD93200" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD93200, dll_base + 0x00053200);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD93230" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD93230, dll_base + 0x00053230);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD93410" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD93410, dll_base + 0x00053410);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a10{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a10{};
			int nType{};
			int a4{};
			int nStatId{};
			int nApplyType{};
			int a7{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a10
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a10);
			setup_data(original_pUnit, original_pItem, original_a10);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, a4, nStatId, nApplyType, a7, nState, fStatList, &moo_a10);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, a4, nStatId, nApplyType, a7, nState, fStatList, &original_a10);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a10, original_a10, "Comparing a10");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD935B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD935B0, dll_base + 0x000535B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD93790" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD93790, dll_base + 0x00053790);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD93A20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD93A20, dll_base + 0x00053A20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD93CB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD93CB0, dll_base + 0x00053CB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD94060" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD94060, dll_base + 0x00054060);
		
		SUBCASE("")
		{
			// Input data
			int moo_pValue{};
			int original_pValue{};
			int nStatId{};

			const auto setup_data = [](
				int& pValue
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pValue);
			setup_data(original_pValue);

			// Call both implementations
			sut(nStatId, &moo_pValue);
			original(nStatId, &original_pValue);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pValue, original_pValue, "Comparing pValue");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD94160" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD94160, dll_base + 0x00054160);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD94190" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD94190, dll_base + 0x00054190);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD943C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD943C0, dll_base + 0x000543C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD944E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD944E0, dll_base + 0x000544E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD94AB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD94AB0, dll_base + 0x00054AB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD94E80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD94E80, dll_base + 0x00054E80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD94F70" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD94F70, dll_base + 0x00054F70);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95050" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD95050, dll_base + 0x00055050);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95200" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD95200, dll_base + 0x00055200);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95210" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD95210, dll_base + 0x00055210);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a9{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a9{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nStatId{};
			int nApplyType{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a9
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a9);
			setup_data(original_pUnit, original_pItem, original_a9);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &moo_a9);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nStatId, nApplyType, nState, fStatList, &original_a9);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a9, original_a9, "Comparing a9");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95430 (#10855)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_AssignProperty, dll_base + 0x00055430);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_a2{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_a2{};
			D2UnitStrc original_pItem{};
			int nType{};
			void* moo_pMods = nullptr;
			void* original_pMods = nullptr;
			int nPropSet{};
			int nApplyType{};

			const auto setup_data = [](
				D2UnitStrc& a2,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a2, moo_pItem);
			setup_data(original_a2, original_pItem);

			// Call both implementations
			sut(nType, &moo_a2, &moo_pItem, moo_pMods, nPropSet, nApplyType);
			original(nType, &original_a2, &original_pItem, original_pMods, nPropSet, nApplyType);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a2, original_a2, "Comparing a2");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pMods, original_pMods, "Comparing pMods");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95810" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD95810, dll_base + 0x00055810);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			void* moo_pMods = nullptr;
			void* original_pMods = nullptr;
			int nIndex{};
			int nPropSet{};
			int nApplyType{};
			D2PropertyStrc pProperty{};
			int nState{};
			int fStatlist{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			sut(nType, &moo_pUnit, &moo_pItem, moo_pMods, nIndex, nPropSet, nApplyType, &pProperty, nState, fStatlist, &moo_a11);
			original(nType, &original_pUnit, &original_pItem, original_pMods, nIndex, nPropSet, nApplyType, &pProperty, nState, fStatlist, &original_a11);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pMods, original_pMods, "Comparing pMods");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD958D0 (#10865)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_ApplyEthereality, dll_base + 0x000558D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem);
			original(&original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD959F0 (#10867)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_UpdateRuneword, dll_base + 0x000559F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			int nUnused{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem);
			setup_data(original_pUnit, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pItem, nUnused);
			const auto original_result = original(&original_pUnit, &original_pItem, nUnused);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95A70" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_UpdateFullSetBoni, dll_base + 0x00055A70);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			int nState{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem);
			setup_data(original_pUnit, original_pItem);

			// Call both implementations
			sut(&moo_pUnit, &moo_pItem, nState);
			original(&original_pUnit, &original_pItem, nState);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95BE0 (#10859)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_CanItemHaveMagicAffix, dll_base + 0x00055BE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			D2MagicAffixTxt pMagicAffixTxtRecord{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &pMagicAffixTxtRecord);
			const auto original_result = original(&original_pItem, &pMagicAffixTxtRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95CC0 (#10860)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_CanItemHaveRareAffix, dll_base + 0x00055CC0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2RareAffixTxt moo_pRareAffixTxtRecord{};
			D2UnitStrc original_pItem{};
			D2RareAffixTxt original_pRareAffixTxtRecord{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2RareAffixTxt& pRareAffixTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pRareAffixTxtRecord);
			setup_data(original_pItem, original_pRareAffixTxtRecord);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pRareAffixTxtRecord);
			const auto original_result = original(&original_pItem, &original_pRareAffixTxtRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pRareAffixTxtRecord, original_pRareAffixTxtRecord, "Comparing pRareAffixTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95D60 (#10861)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_CanItemBeHighQuality, dll_base + 0x00055D60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2QualityItemsTxt moo_pQualityItemsTxtRecord{};
			D2UnitStrc original_pItem{};
			D2QualityItemsTxt original_pQualityItemsTxtRecord{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2QualityItemsTxt& pQualityItemsTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pQualityItemsTxtRecord);
			setup_data(original_pItem, original_pQualityItemsTxtRecord);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pQualityItemsTxtRecord);
			const auto original_result = original(&original_pItem, &original_pQualityItemsTxtRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pQualityItemsTxtRecord, original_pQualityItemsTxtRecord, "Comparing pQualityItemsTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95E90 (#10862)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_SetRandomElixirFileIndex, dll_base + 0x00055E90);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem);
			original(&original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95F90 (#10868)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_AddCraftPropertyList, dll_base + 0x00055F90);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2PropertyStrc moo_pProperty{};
			D2UnitStrc original_pItem{};
			D2PropertyStrc original_pProperty{};
			int nUnused{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2PropertyStrc& pProperty
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pProperty);
			setup_data(original_pItem, original_pProperty);

			// Call both implementations
			sut(&moo_pItem, &moo_pProperty, nUnused);
			original(&original_pItem, &original_pProperty, nUnused);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pProperty, original_pProperty, "Comparing pProperty");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD95FC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc01, dll_base + 0x00055FC0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD96110" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_AddPropertyToItemStatList, dll_base + 0x00056110);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_pUnused{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_pUnused{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& pUnused
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_pUnused);
			setup_data(original_pUnit, original_pItem, original_pUnused);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_pUnused);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_pUnused);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pUnused, original_pUnused, "Comparing pUnused");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD96210" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc02, dll_base + 0x00056210);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD96350" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc03, dll_base + 0x00056350);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD964A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc04, dll_base + 0x000564A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD965F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc05, dll_base + 0x000565F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD96880" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc06, dll_base + 0x00056880);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD96B00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc07, dll_base + 0x00056B00);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD96DA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc08, dll_base + 0x00056DA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD96EE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc09, dll_base + 0x00056EE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97040" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc24, dll_base + 0x00057040);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97180" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc10, dll_base + 0x00057180);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD972E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc11, dll_base + 0x000572E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97430" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc14, dll_base + 0x00057430);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD975F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc19, dll_base + 0x000575F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97830" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc18, dll_base + 0x00057830);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97920" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc15, dll_base + 0x00057920);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD979A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc16, dll_base + 0x000579A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97A20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc17, dll_base + 0x00057A20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97BA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc20, dll_base + 0x00057BA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97C20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc21, dll_base + 0x00057C20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97D50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc22, dll_base + 0x00057D50);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97E80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc12, dll_base + 0x00057E80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD97FB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc13, dll_base + 0x00057FB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98120" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_PropertyFunc23, dll_base + 0x00058120);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a11{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a11{};
			int nType{};
			D2PropertyStrc pProperty{};
			int nSet{};
			short nStatId{};
			int nLayer{};
			int nValue{};
			int nState{};
			int fStatList{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a11
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a11);
			setup_data(original_pUnit, original_pItem, original_a11);

			// Call both implementations
			const auto moo_result = sut(nType, &moo_pUnit, &moo_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &moo_a11);
			const auto original_result = original(nType, &original_pUnit, &original_pItem, &pProperty, nSet, nStatId, nLayer, nValue, nState, fStatList, &original_a11);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_a11, original_a11, "Comparing a11");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98160 (#11292)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11292_ItemAssignProperty, dll_base + 0x00058160);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_a10{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_a10{};
			int nType{};
			void* moo_pMods = nullptr;
			void* original_pMods = nullptr;
			int nIndex{};
			int nPropSet{};
			D2PropertyStrc pProperty{};
			int nState{};
			int fStatlist{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2UnitStrc& a10
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_a10);
			setup_data(original_pUnit, original_pItem, original_a10);

			// Call both implementations
			sut(nType, &moo_pUnit, &moo_pItem, moo_pMods, nIndex, nPropSet, &pProperty, nState, fStatlist, &moo_a10);
			original(nType, &original_pUnit, &original_pItem, original_pMods, nIndex, nPropSet, &pProperty, nState, fStatlist, &original_a10);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pMods, original_pMods, "Comparing pMods");
			SKIP_MOO_CHECK_EQ(moo_a10, original_a10, "Comparing a10");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98220" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD98220, dll_base + 0x00058220);
		
		SUBCASE("")
		{
			int nMin{};
			int nMax{};
			int nUnused{};
			void* moo_pUserData = nullptr;
			void* original_pUserData = nullptr;

			// Call both implementations
			const auto moo_result = sut(nMin, nMax, nUnused, moo_pUserData);
			const auto original_result = original(nMin, nMax, nUnused, original_pUserData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUserData, original_pUserData, "Comparing pUserData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD982A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD982A0, dll_base + 0x000582A0);
		
		SUBCASE("")
		{
			int nStatId{};
			int a2{};
			int nUnused{};
			void* moo_pUserData = nullptr;
			void* original_pUserData = nullptr;

			// Call both implementations
			const auto moo_result = sut(nStatId, a2, nUnused, moo_pUserData);
			const auto original_result = original(nStatId, a2, nUnused, original_pUserData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUserData, original_pUserData, "Comparing pUserData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98300 (#11300)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMMODS_EvaluateItemFormula, dll_base + 0x00058300);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			unsigned int nCalc{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem);
			setup_data(original_pUnit, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pItem, nCalc);
			const auto original_result = original(&original_pUnit, &original_pItem, nCalc);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
}
#endif
