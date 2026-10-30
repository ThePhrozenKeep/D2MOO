#include <D2CommonTestDefines.h>

#ifdef ANIM_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/AnimTbls.h>
#include <Units/Units.h>


TEST_SUITE("AnimTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD473C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadAnimDataD2, dll_base + 0x000073C0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			const auto moo_result = sut(hArchive);
			const auto original_result = original(hArchive);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD47460" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadAnimDataD2, dll_base + 0x00007460);
		
		SUBCASE("")
		{
			// Input data
			D2AnimDataTableStrc moo_pAnimData{};
			D2AnimDataTableStrc original_pAnimData{};

			const auto setup_data = [](
				D2AnimDataTableStrc& pAnimData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAnimData);
			setup_data(original_pAnimData);

			// Call both implementations
			sut(&moo_pAnimData);
			original(&original_pAnimData);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAnimData, original_pAnimData, "Comparing pAnimData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD474A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetAnimDataRecord, dll_base + 0x000074A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc original_pUnit{};
			D2InventoryStrc original_pInventory{};
			int nClassId{};
			int nMode{};
			int nUnitType{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pInventory);
			setup_data(original_pUnit, original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nClassId, nMode, nUnitType, &moo_pInventory);
			const auto original_result = original(&original_pUnit, nClassId, nMode, nUnitType, &original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD475D0 (#10640)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnitAnimInfoDebugSet, dll_base + 0x000075D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nAnimSpeed{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nAnimSpeed);
			original(&original_pUnit, nAnimSpeed);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD47700 (#10641)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetAnimDataInfo, dll_base + 0x00007700);
		
		SUBCASE("")
		{
			// Input data
			char moo_szPath{};
			int moo_pOutLength{};
			int moo_pOutAnimSpeed{};
			int moo_pOutFirstFrameTagged{};
			char original_szPath{};
			int original_pOutLength{};
			int original_pOutAnimSpeed{};
			int original_pOutFirstFrameTagged{};

			const auto setup_data = [](
				char& szPath,
				int& pOutLength,
				int& pOutAnimSpeed,
				int& pOutFirstFrameTagged
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_szPath, moo_pOutLength, moo_pOutAnimSpeed, moo_pOutFirstFrameTagged);
			setup_data(original_szPath, original_pOutLength, original_pOutAnimSpeed, original_pOutFirstFrameTagged);

			// Call both implementations
			const auto moo_result = sut(&moo_szPath, &moo_pOutLength, &moo_pOutAnimSpeed, &moo_pOutFirstFrameTagged);
			const auto original_result = original(&original_szPath, &original_pOutLength, &original_pOutAnimSpeed, &original_pOutFirstFrameTagged);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_szPath, original_szPath, "Comparing szPath");
			SKIP_MOO_CHECK_EQ(moo_pOutLength, original_pOutLength, "Comparing pOutLength");
			SKIP_MOO_CHECK_EQ(moo_pOutAnimSpeed, original_pOutAnimSpeed, "Comparing pOutAnimSpeed");
			SKIP_MOO_CHECK_EQ(moo_pOutFirstFrameTagged, original_pOutFirstFrameTagged, "Comparing pOutFirstFrameTagged");
		}
	}
}
#endif
