#include <D2CommonTestDefines.h>

#ifdef COMPOSIT_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Composit.h>
#include <Units/Units.h>


TEST_SUITE("D2CompositTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD466C0 (#10884)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10884_COMPOSIT_unk, dll_base + 0x000066C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2InventoryStrc moo_pInventory{};
			char moo_szPath{};
			int moo_pWeaponClassCode{};
			D2UnitStrc original_pUnit{};
			D2InventoryStrc original_pInventory{};
			char original_szPath{};
			int original_pWeaponClassCode{};
			int nClass{};
			int nMode{};
			int nUnitType{};
			BOOL bAddPathPrefix{};
			int a9{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2InventoryStrc& pInventory,
				char& szPath,
				int& pWeaponClassCode
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pInventory, moo_szPath, moo_pWeaponClassCode);
			setup_data(original_pUnit, original_pInventory, original_szPath, original_pWeaponClassCode);

			// Call both implementations
			sut(&moo_pUnit, nClass, nMode, nUnitType, &moo_pInventory, &moo_szPath, &moo_pWeaponClassCode, bAddPathPrefix, a9);
			original(&original_pUnit, nClass, nMode, nUnitType, &original_pInventory, &original_szPath, &original_pWeaponClassCode, bAddPathPrefix, a9);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_szPath, original_szPath, "Comparing szPath");
			SKIP_MOO_CHECK_EQ(moo_pWeaponClassCode, original_pWeaponClassCode, "Comparing pWeaponClassCode");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD46BC0 (#10885)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10885_COMPOSIT_unk, dll_base + 0x00006BC0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			char moo_szPath{};
			int moo_pWeaponClassCode{};
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc original_pUnit{};
			char original_szPath{};
			int original_pWeaponClassCode{};
			D2InventoryStrc original_pInventory{};
			BOOL bAddPathPrefix{};
			int a5{};
			int nAnimMode{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				char& szPath,
				int& pWeaponClassCode,
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_szPath, moo_pWeaponClassCode, moo_pInventory);
			setup_data(original_pUnit, original_szPath, original_pWeaponClassCode, original_pInventory);

			// Call both implementations
			sut(&moo_pUnit, &moo_szPath, &moo_pWeaponClassCode, bAddPathPrefix, a5, &moo_pInventory, nAnimMode);
			original(&original_pUnit, &original_szPath, &original_pWeaponClassCode, bAddPathPrefix, a5, &original_pInventory, nAnimMode);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_szPath, original_szPath, "Comparing szPath");
			SKIP_MOO_CHECK_EQ(moo_pWeaponClassCode, original_pWeaponClassCode, "Comparing pWeaponClassCode");
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD46C60 (#10886)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_GetWeaponClassIdFromCode, dll_base + 0x00006C60);
		
		SUBCASE("")
		{
			int nWeaponClassCode{};

			// Call both implementations
			const auto moo_result = sut(nWeaponClassCode);
			const auto original_result = original(nWeaponClassCode);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD46C90 (#10887)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_GetWeaponClassCode, dll_base + 0x00006C90);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2InventoryStrc moo_pInventory{};
			int moo_pWeaponClassId{};
			D2UnitStrc original_pUnit{};
			D2InventoryStrc original_pInventory{};
			int original_pWeaponClassId{};
			int nUnitType{};
			int nClass{};
			int nMode{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2InventoryStrc& pInventory,
				int& pWeaponClassId
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pInventory, moo_pWeaponClassId);
			setup_data(original_pUnit, original_pInventory, original_pWeaponClassId);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nUnitType, nClass, nMode, &moo_pInventory, &moo_pWeaponClassId);
			const auto original_result = original(&original_pUnit, nUnitType, nClass, nMode, &original_pInventory, &original_pWeaponClassId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pWeaponClassId, original_pWeaponClassId, "Comparing pWeaponClassId");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD47150 (#10888)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_GetWeaponClassId, dll_base + 0x00007150);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2InventoryStrc moo_pInventory{};
			int moo_pWeaponClassId{};
			D2UnitStrc original_pUnit{};
			D2InventoryStrc original_pInventory{};
			int original_pWeaponClassId{};
			int nAnimMode{};
			BOOL a5{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2InventoryStrc& pInventory,
				int& pWeaponClassId
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pInventory, moo_pWeaponClassId);
			setup_data(original_pUnit, original_pInventory, original_pWeaponClassId);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pInventory, &moo_pWeaponClassId, nAnimMode, a5);
			const auto original_result = original(&original_pUnit, &original_pInventory, &original_pWeaponClassId, nAnimMode, a5);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pWeaponClassId, original_pWeaponClassId, "Comparing pWeaponClassId");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD47200 (#10889)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_IsArmorComponent, dll_base + 0x00007200);
		
		SUBCASE("")
		{
			int nComponent{};

			// Call both implementations
			const auto moo_result = sut(nComponent);
			const auto original_result = original(nComponent);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD47230 (#10890)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_IsWeaponBowOrXBow, dll_base + 0x00007230);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD472E0 (#10891)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_GetArmorTypeFromComponent, dll_base + 0x000072E0);
		
		SUBCASE("")
		{
			// Input data
			uint8_t moo_pArmorComponents{};
			uint8_t original_pArmorComponents{};
			int nComponent{};

			const auto setup_data = [](
				uint8_t& pArmorComponents
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pArmorComponents);
			setup_data(original_pArmorComponents);

			// Call both implementations
			const auto moo_result = sut(nComponent, &moo_pArmorComponents);
			const auto original_result = original(nComponent, &original_pArmorComponents);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pArmorComponents, original_pArmorComponents, "Comparing pArmorComponents");
		}
	}
}
#endif
