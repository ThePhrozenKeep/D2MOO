#include <D2CommonTestDefines.h>

#ifdef INV_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/InvTbls.h>

#include <Fixtures/DataTbls/Fixtures.h>


TEST_SUITE("InvTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD542D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadInventoryTxt, dll_base + 0x000142D0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD54F10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadInventoryTxt, dll_base + 0x00014F10);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(InventoryTxtFixture<NoOpFixture>, "D2Common.0x6FD54F20 (#10635)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetInventoryRect, dll_base + 0x00014F20);

		// The native bound is inclusive (unsigned nIndex <= nInventoryTxtRecordCount): advertise one record fewer
		// than loaded so that the last loaded record sits exactly on the bound.
		const auto original_record_count = reinterpret_cast<int*>(dll_base + 0x000A9608 + 0x00000CCC);
		sgptDataTables->nInventoryTxtRecordCount = inventory_record_count - 1;
		*original_record_count = inventory_record_count - 1;

		for (int nInventoryTxtId = 0; nInventoryTxtId < inventory_record_count; ++nInventoryTxtId)
		{
			CAPTURE(nInventoryTxtId);
			D2InvRectStrc moo_pInvRect{};
			D2InvRectStrc original_pInvRect{};

			// Call both implementations
			sut(nInventoryTxtId, FALSE, &moo_pInvRect);
			original(nInventoryTxtId, FALSE, &original_pInvRect);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pInvRect, original_pInvRect, "Comparing pInvRect");
		}

		sgptDataTables->nInventoryTxtRecordCount = inventory_record_count;
		*original_record_count = inventory_record_count;
	}
	
	TEST_CASE_FIXTURE(InventoryTxtFixture<NoOpFixture>, "D2Common.0x6FD54FB0 (#10636)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetInventoryGridInfo, dll_base + 0x00014FB0);

		// The native bound is inclusive (unsigned nIndex <= nInventoryTxtRecordCount): advertise one record fewer
		// than loaded so that the last loaded record sits exactly on the bound.
		const auto original_record_count = reinterpret_cast<int*>(dll_base + 0x000A9608 + 0x00000CCC);
		sgptDataTables->nInventoryTxtRecordCount = inventory_record_count - 1;
		*original_record_count = inventory_record_count - 1;

		for (int nInventoryTxtId = 0; nInventoryTxtId < inventory_record_count; ++nInventoryTxtId)
		{
			CAPTURE(nInventoryTxtId);
			D2InventoryGridInfoStrc moo_pInventoryGridInfo{};
			D2InventoryGridInfoStrc original_pInventoryGridInfo{};

			// Call both implementations
			sut(nInventoryTxtId, FALSE, &moo_pInventoryGridInfo);
			original(nInventoryTxtId, FALSE, &original_pInventoryGridInfo);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pInventoryGridInfo, original_pInventoryGridInfo, "Comparing pInventoryGridInfo");
		}

		sgptDataTables->nInventoryTxtRecordCount = inventory_record_count;
		*original_record_count = inventory_record_count;
	}
	
	TEST_CASE_FIXTURE(InventoryTxtFixture<NoOpFixture>, "D2Common.0x6FD55030 (#10637)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetInventoryComponentGrid, dll_base + 0x00015030);

		// The native bound is inclusive (unsigned nIndex <= nInventoryTxtRecordCount): advertise one record fewer
		// than loaded so that the last loaded record sits exactly on the bound.
		const auto original_record_count = reinterpret_cast<int*>(dll_base + 0x000A9608 + 0x00000CCC);
		sgptDataTables->nInventoryTxtRecordCount = inventory_record_count - 1;
		*original_record_count = inventory_record_count - 1;

		for (int nInventoryTxtId = 0; nInventoryTxtId < inventory_record_count; ++nInventoryTxtId)
		{
			CAPTURE(nInventoryTxtId);
			for (int nComponent = 0; nComponent < NUM_INV_COMPS; ++nComponent)
			{
				CAPTURE(nComponent);
				D2InvCompGridStrc moo_pInvCompGrid{};
				D2InvCompGridStrc original_pInvCompGrid{};

				// Call both implementations
				sut(nInventoryTxtId, FALSE, &moo_pInvCompGrid, nComponent);
				original(nInventoryTxtId, FALSE, &original_pInvCompGrid, nComponent);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pInvCompGrid, original_pInvCompGrid, "Comparing pInvCompGrid");
			}
		}

		sgptDataTables->nInventoryTxtRecordCount = inventory_record_count;
		*original_record_count = inventory_record_count;
	}
}
#endif
