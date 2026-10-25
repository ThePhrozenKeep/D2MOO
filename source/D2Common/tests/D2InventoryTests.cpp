#include <D2CommonTestDefines.h>

#ifdef INVENTORY_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Inventory.h>
#include <DataTbls/InvTbls.h>
#include <Units/Units.h>


TEST_SUITE("D2InventoryTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E210" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_RemoveItem, dll_base + 0x0004E210);
		
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
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E4A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemExtraDataFromItem, dll_base + 0x0004E4A0);
		
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
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E4C0 (#10240)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_AllocInventory, dll_base + 0x0004E4C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pOwner{};
			D2UnitStrc original_pOwner{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2UnitStrc& pOwner
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pOwner);
			setup_data(original_pOwner);

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool, &moo_pOwner);
			const auto original_result = original(original_pMemPool, &original_pOwner);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pOwner, original_pOwner, "Comparing pOwner");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E520 (#10241)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_FreeInventory, dll_base + 0x0004E520);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			sut(&moo_pInventory);
			original(&original_pInventory);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E620 (#10244)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CompareWithItemsParentInventory, dll_base + 0x0004E620);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem);
			const auto original_result = original(&original_pInventory, &original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E660 (#10243)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_RemoveItemFromInventory, dll_base + 0x0004E660);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem);
			const auto original_result = original(&original_pInventory, &original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E6A0 (#10242)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_PlaceItemInSocket, dll_base + 0x0004E6A0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			int nUnused{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nUnused);
			const auto original_result = original(&original_pInventory, &original_pItem, nUnused);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E7A0 (#10277)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetFirstItem, dll_base + 0x0004E7A0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E7C0 (#10278)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetLastItem, dll_base + 0x0004E7C0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8E7E0 (#10245)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetFreePosition, dll_base + 0x0004E7E0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			int moo_pFreeX{};
			int moo_pFreeY{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			int original_pFreeX{};
			int original_pFreeY{};
			int nInventoryRecordId{};
			uint8_t nPage{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem,
				int& pFreeX,
				int& pFreeY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem, moo_pFreeX, moo_pFreeY);
			setup_data(original_pInventory, original_pItem, original_pFreeX, original_pFreeY);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nInventoryRecordId, &moo_pFreeX, &moo_pFreeY, nPage);
			const auto original_result = original(&original_pInventory, &original_pItem, nInventoryRecordId, &original_pFreeX, &original_pFreeY, nPage);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pFreeX, original_pFreeX, "Comparing pFreeX");
			SKIP_MOO_CHECK_EQ(moo_pFreeY, original_pFreeY, "Comparing pFreeY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8EAF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetGrid, dll_base + 0x0004EAF0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryGridInfoStrc moo_pInventoryGridInfo{};
			D2InventoryStrc original_pInventory{};
			D2InventoryGridInfoStrc original_pInventoryGridInfo{};
			int nInventoryGrid{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2InventoryGridInfoStrc& pInventoryGridInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pInventoryGridInfo);
			setup_data(original_pInventory, original_pInventoryGridInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nInventoryGrid, &moo_pInventoryGridInfo);
			const auto original_result = original(&original_pInventory, nInventoryGrid, &original_pInventoryGridInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pInventoryGridInfo, original_pInventoryGridInfo, "Comparing pInventoryGridInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8EC70" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CanItemBePlacedAtPos, dll_base + 0x0004EC70);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryGridStrc moo_pInventoryGrid{};
			D2InventoryGridStrc original_pInventoryGrid{};
			int nX{};
			int nY{};
			uint8_t nItemWidth{};
			uint8_t nItemHeight{};

			const auto setup_data = [](
				D2InventoryGridStrc& pInventoryGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventoryGrid);
			setup_data(original_pInventoryGrid);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventoryGrid, nX, nY, nItemWidth, nItemHeight);
			const auto original_result = original(&original_pInventoryGrid, nX, nY, nItemWidth, nItemHeight);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventoryGrid, original_pInventoryGrid, "Comparing pInventoryGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8ECF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_FindFreePositionBottomRightToTopLeftWithWeight, dll_base + 0x0004ECF0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryGridStrc moo_pInventoryGrid{};
			int moo_pFreeX{};
			int moo_pFreeY{};
			D2InventoryGridStrc original_pInventoryGrid{};
			int original_pFreeX{};
			int original_pFreeY{};
			uint8_t nItemWidth{};
			uint8_t nItemHeight{};

			const auto setup_data = [](
				D2InventoryGridStrc& pInventoryGrid,
				int& pFreeX,
				int& pFreeY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventoryGrid, moo_pFreeX, moo_pFreeY);
			setup_data(original_pInventoryGrid, original_pFreeX, original_pFreeY);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventoryGrid, &moo_pFreeX, &moo_pFreeY, nItemWidth, nItemHeight);
			const auto original_result = original(&original_pInventoryGrid, &original_pFreeX, &original_pFreeY, nItemWidth, nItemHeight);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventoryGrid, original_pInventoryGrid, "Comparing pInventoryGrid");
			SKIP_MOO_CHECK_EQ(moo_pFreeX, original_pFreeX, "Comparing pFreeX");
			SKIP_MOO_CHECK_EQ(moo_pFreeY, original_pFreeY, "Comparing pFreeY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8EE20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetPlacementWeight, dll_base + 0x0004EE20);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryGridStrc moo_pInventoryGrid{};
			D2InventoryGridStrc original_pInventoryGrid{};
			int nXPos{};
			int nYPos{};
			uint8_t nItemWidth{};
			uint8_t nItemHeight{};

			const auto setup_data = [](
				D2InventoryGridStrc& pInventoryGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventoryGrid);
			setup_data(original_pInventoryGrid);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventoryGrid, nXPos, nYPos, nItemWidth, nItemHeight);
			const auto original_result = original(&original_pInventoryGrid, nXPos, nYPos, nItemWidth, nItemHeight);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventoryGrid, original_pInventoryGrid, "Comparing pInventoryGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8EFB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_FindFreePositionTopLeftToBottomRightWithWeight, dll_base + 0x0004EFB0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryGridStrc moo_pInventoryGrid{};
			int moo_pFreeX{};
			int moo_pFreeY{};
			D2InventoryGridStrc original_pInventoryGrid{};
			int original_pFreeX{};
			int original_pFreeY{};
			uint8_t nItemWidth{};
			uint8_t nItemHeight{};

			const auto setup_data = [](
				D2InventoryGridStrc& pInventoryGrid,
				int& pFreeX,
				int& pFreeY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventoryGrid, moo_pFreeX, moo_pFreeY);
			setup_data(original_pInventoryGrid, original_pFreeX, original_pFreeY);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventoryGrid, &moo_pFreeX, &moo_pFreeY, nItemWidth, nItemHeight);
			const auto original_result = original(&original_pInventoryGrid, &original_pFreeX, &original_pFreeY, nItemWidth, nItemHeight);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventoryGrid, original_pInventoryGrid, "Comparing pInventoryGrid");
			SKIP_MOO_CHECK_EQ(moo_pFreeX, original_pFreeX, "Comparing pFreeX");
			SKIP_MOO_CHECK_EQ(moo_pFreeY, original_pFreeY, "Comparing pFreeY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8F0E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_FindFreePositionTopLeftToBottomRight, dll_base + 0x0004F0E0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryGridStrc moo_pInventoryGrid{};
			int moo_pFreeX{};
			int moo_pFreeY{};
			D2InventoryGridStrc original_pInventoryGrid{};
			int original_pFreeX{};
			int original_pFreeY{};
			uint8_t nItemWidth{};
			uint8_t nItemHeight{};

			const auto setup_data = [](
				D2InventoryGridStrc& pInventoryGrid,
				int& pFreeX,
				int& pFreeY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventoryGrid, moo_pFreeX, moo_pFreeY);
			setup_data(original_pInventoryGrid, original_pFreeX, original_pFreeY);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventoryGrid, &moo_pFreeX, &moo_pFreeY, nItemWidth, nItemHeight);
			const auto original_result = original(&original_pInventoryGrid, &original_pFreeX, &original_pFreeY, nItemWidth, nItemHeight);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventoryGrid, original_pInventoryGrid, "Comparing pInventoryGrid");
			SKIP_MOO_CHECK_EQ(moo_pFreeX, original_pFreeX, "Comparing pFreeX");
			SKIP_MOO_CHECK_EQ(moo_pFreeY, original_pFreeY, "Comparing pFreeY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8F1E0 (#10246)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_PlaceItemAtFreePosition, dll_base + 0x0004F1E0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			int nInventoryRecordId{};
			BOOL bUnused{};
			uint8_t nPage{};
			char szFile{};
			int nLine{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nInventoryRecordId, bUnused, nPage, &szFile, nLine);
			const auto original_result = original(&original_pInventory, &original_pItem, nInventoryRecordId, bUnused, nPage, &szFile, nLine);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8F250" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_PlaceItemInGrid, dll_base + 0x0004F250);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			int nXPos{};
			int nYPos{};
			int nInventoryGrid{};
			int nInventoryRecordId{};
			BOOL bUnused{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nXPos, nYPos, nInventoryGrid, nInventoryRecordId, bUnused);
			const auto original_result = original(&original_pInventory, &original_pItem, nXPos, nYPos, nInventoryGrid, nInventoryRecordId, bUnused);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8F600 (#10247)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CanItemBePlaced, dll_base + 0x0004F600);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc* moo_ppExchangeItem{};
			unsigned int moo_pHoveredItems{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			D2UnitStrc* original_ppExchangeItem{};
			unsigned int original_pHoveredItems{};
			int nXPos{};
			int nYPos{};
			int nInventoryRecordId{};
			uint8_t nPage{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem,
				D2UnitStrc*& ppExchangeItem,
				unsigned int& pHoveredItems
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem, moo_ppExchangeItem, moo_pHoveredItems);
			setup_data(original_pInventory, original_pItem, original_ppExchangeItem, original_pHoveredItems);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nXPos, nYPos, nInventoryRecordId, &moo_ppExchangeItem, &moo_pHoveredItems, nPage);
			const auto original_result = original(&original_pInventory, &original_pItem, nXPos, nYPos, nInventoryRecordId, &original_ppExchangeItem, &original_pHoveredItems, nPage);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_ppExchangeItem, original_ppExchangeItem, "Comparing ppExchangeItem");
			SKIP_MOO_CHECK_EQ(moo_pHoveredItems, original_pHoveredItems, "Comparing pHoveredItems");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8F780 (#10248)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CanItemsBeExchanged, dll_base + 0x0004F780);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc* moo_ppExchangeItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			D2UnitStrc* original_ppExchangeItem{};
			int nXPos{};
			int nYPos{};
			int nInventoryRecordId{};
			uint8_t nPage{};
			BOOL bCheckIfCube{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem,
				D2UnitStrc*& ppExchangeItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem, moo_ppExchangeItem);
			setup_data(original_pInventory, original_pItem, original_ppExchangeItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nXPos, nYPos, nInventoryRecordId, &moo_ppExchangeItem, nPage, bCheckIfCube);
			const auto original_result = original(&original_pInventory, &original_pItem, nXPos, nYPos, nInventoryRecordId, &original_ppExchangeItem, nPage, bCheckIfCube);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_ppExchangeItem, original_ppExchangeItem, "Comparing ppExchangeItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8F930 (#10249)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_PlaceItemAtInventoryPage, dll_base + 0x0004F930);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			int nXPos{};
			int nYPos{};
			int nInventoryRecordId{};
			BOOL bUnused{};
			uint8_t nPage{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nXPos, nYPos, nInventoryRecordId, bUnused, nPage);
			const auto original_result = original(&original_pInventory, &original_pItem, nXPos, nYPos, nInventoryRecordId, bUnused, nPage);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8F970 (#10250)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_Return, dll_base + 0x0004F970);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			char szFile{};
			int nLine{};
			int nX{};
			int nY{};
			int nInventoryRecordId{};
			BOOL bClient{};
			uint8_t nPage{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			sut(&szFile, nLine, &moo_pInventory, nX, nY, nInventoryRecordId, bClient, nPage);
			original(&szFile, nLine, &original_pInventory, nX, nY, nInventoryRecordId, bClient, nPage);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8F980 (#10252)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemFromInventoryPage, dll_base + 0x0004F980);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			int moo_pX{};
			int moo_pY{};
			D2InventoryStrc original_pInventory{};
			int original_pX{};
			int original_pY{};
			int nGridX{};
			int nGridY{};
			int nInventoryRecordId{};
			uint8_t nPage{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pX, moo_pY);
			setup_data(original_pInventory, original_pX, original_pY);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nGridX, nGridY, &moo_pX, &moo_pY, nInventoryRecordId, nPage);
			const auto original_result = original(&original_pInventory, nGridX, nGridY, &original_pX, &original_pY, nInventoryRecordId, nPage);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FAB0 (#10253)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_PlaceItemInBodyLoc, dll_base + 0x0004FAB0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			int nBodyLoc{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nBodyLoc);
			const auto original_result = original(&original_pInventory, &original_pItem, nBodyLoc);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FAE0 (#10257)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemFromBodyLoc, dll_base + 0x0004FAE0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			int nBodyLoc{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nBodyLoc);
			const auto original_result = original(&original_pInventory, nBodyLoc);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FB20 (#10255)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetSecondWieldingWeapon, dll_base + 0x0004FB20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc* moo_ppItem{};
			D2UnitStrc original_pPlayer{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc* original_ppItem{};
			int nBodyLoc{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer,
				D2InventoryStrc& pInventory,
				D2UnitStrc*& ppItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer, moo_pInventory, moo_ppItem);
			setup_data(original_pPlayer, original_pInventory, original_ppItem);

			// Call both implementations
			sut(&moo_pPlayer, &moo_pInventory, &moo_ppItem, nBodyLoc);
			original(&original_pPlayer, &original_pInventory, &original_ppItem, nBodyLoc);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_ppItem, original_ppItem, "Comparing ppItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FBB0 (#10256)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CheckEquipmentForWeaponByClass, dll_base + 0x0004FBB0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			int nWeaponClass{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nWeaponClass);
			const auto original_result = original(&original_pInventory, nWeaponClass);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FC60 (#10258)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetLeftHandWeapon, dll_base + 0x0004FC60);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FD10 (#11301)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetSecondaryWeapon, dll_base + 0x0004FD10);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FDD0 (#10259)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetCompositItem, dll_base + 0x0004FDD0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			int nComponent{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nComponent);
			const auto original_result = original(&original_pInventory, nComponent);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FE80 (#10260)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetBodyLocFromEquippedItem, dll_base + 0x0004FE80);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem);
			const auto original_result = original(&original_pInventory, &original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FED0 (#11278)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemsXPosition, dll_base + 0x0004FED0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem);
			const auto original_result = original(&original_pInventory, &original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FF20 (#10261)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_SetCursorItem, dll_base + 0x0004FF20);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			sut(&moo_pInventory, &moo_pItem);
			original(&original_pInventory, &original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FF80 (#10262)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetCursorItem, dll_base + 0x0004FF80);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD8FFA0 (#10263)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_FindBackPackItemForStack, dll_base + 0x0004FFA0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pStackable{};
			D2UnitStrc moo_pCheckItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pStackable{};
			D2UnitStrc original_pCheckItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pStackable,
				D2UnitStrc& pCheckItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pStackable, moo_pCheckItem);
			setup_data(original_pInventory, original_pStackable, original_pCheckItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pStackable, &moo_pCheckItem);
			const auto original_result = original(&original_pInventory, &original_pStackable, &original_pCheckItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pStackable, original_pStackable, "Comparing pStackable");
			SKIP_MOO_CHECK_EQ(moo_pCheckItem, original_pCheckItem, "Comparing pCheckItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90080 (#10264)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_FindEquippedItemForStack, dll_base + 0x00050080);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pStackable{};
			D2UnitStrc moo_pCheckItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pStackable{};
			D2UnitStrc original_pCheckItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pStackable,
				D2UnitStrc& pCheckItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pStackable, moo_pCheckItem);
			setup_data(original_pInventory, original_pStackable, original_pCheckItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pStackable, &moo_pCheckItem);
			const auto original_result = original(&original_pInventory, &original_pStackable, &original_pCheckItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pStackable, original_pStackable, "Comparing pStackable");
			SKIP_MOO_CHECK_EQ(moo_pCheckItem, original_pCheckItem, "Comparing pCheckItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90130 (#10265)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_FindFillableBook, dll_base + 0x00050130);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pScrolls{};
			D2UnitStrc moo_pCheckItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pScrolls{};
			D2UnitStrc original_pCheckItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pScrolls,
				D2UnitStrc& pCheckItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pScrolls, moo_pCheckItem);
			setup_data(original_pInventory, original_pScrolls, original_pCheckItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pScrolls, &moo_pCheckItem);
			const auto original_result = original(&original_pInventory, &original_pScrolls, &original_pCheckItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pScrolls, original_pScrolls, "Comparing pScrolls");
			SKIP_MOO_CHECK_EQ(moo_pCheckItem, original_pCheckItem, "Comparing pCheckItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90230 (#10266)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_PlaceItemInBeltSlot, dll_base + 0x00050230);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			int nSlot{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nSlot);
			const auto original_result = original(&original_pInventory, &original_pItem, nSlot);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD902B0 (#10268)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_HasSimilarPotionInBelt, dll_base + 0x000502B0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pPotion{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pPotion{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pPotion
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pPotion);
			setup_data(original_pInventory, original_pPotion);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pPotion);
			const auto original_result = original(&original_pInventory, &original_pPotion);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pPotion, original_pPotion, "Comparing pPotion");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90340 (#10269)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetFreeBeltSlot, dll_base + 0x00050340);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			int moo_pFreeSlotId{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			int original_pFreeSlotId{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem,
				int& pFreeSlotId
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem, moo_pFreeSlotId);
			setup_data(original_pInventory, original_pItem, original_pFreeSlotId);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, &moo_pFreeSlotId);
			const auto original_result = original(&original_pInventory, &original_pItem, &original_pFreeSlotId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pFreeSlotId, original_pFreeSlotId, "Comparing pFreeSlotId");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD904F0 (#10270)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_PlaceItemInFreeBeltSlot, dll_base + 0x000504F0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem);
			const auto original_result = original(&original_pInventory, &original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90550 (#10271)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemFromBeltSlot, dll_base + 0x00050550);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			int nSlotId{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nSlotId);
			const auto original_result = original(&original_pInventory, nSlotId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90590 (#10272)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetUseableItemFromBeltSlot, dll_base + 0x00050590);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc* moo_ppItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			D2UnitStrc* original_ppItem{};
			int nSlotId{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem,
				D2UnitStrc*& ppItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem, moo_ppItem);
			setup_data(original_pInventory, original_pItem, original_ppItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nSlotId, &moo_ppItem);
			const auto original_result = original(&original_pInventory, &original_pItem, nSlotId, &original_ppItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_ppItem, original_ppItem, "Comparing ppItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90690 (#10273)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetEquippedShield, dll_base + 0x00050690);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc* moo_ppItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc* original_ppItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc*& ppItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_ppItem);
			setup_data(original_pInventory, original_ppItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_ppItem);
			const auto original_result = original(&original_pInventory, &original_ppItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_ppItem, original_ppItem, "Comparing ppItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90760 (#10274)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetEquippedWeapon, dll_base + 0x00050760);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc* moo_ppItem{};
			int moo_pBodyLoc{};
			BOOL moo_pIsLeftHandItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc* original_ppItem{};
			int original_pBodyLoc{};
			BOOL original_pIsLeftHandItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc*& ppItem,
				int& pBodyLoc,
				BOOL& pIsLeftHandItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_ppItem, moo_pBodyLoc, moo_pIsLeftHandItem);
			setup_data(original_pInventory, original_ppItem, original_pBodyLoc, original_pIsLeftHandItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_ppItem, &moo_pBodyLoc, &moo_pIsLeftHandItem);
			const auto original_result = original(&original_pInventory, &original_ppItem, &original_pBodyLoc, &original_pIsLeftHandItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_ppItem, original_ppItem, "Comparing ppItem");
			SKIP_MOO_CHECK_EQ(moo_pBodyLoc, original_pBodyLoc, "Comparing pBodyLoc");
			SKIP_MOO_CHECK_EQ(moo_pIsLeftHandItem, original_pIsLeftHandItem, "Comparing pIsLeftHandItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90850 (#10275)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_HasBodyArmorEquipped, dll_base + 0x00050850);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD908A0 (#10276)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_IsItemBodyLocFree, dll_base + 0x000508A0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};
			int nBodyLoc{};
			int nInventoryRecordId{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem, nBodyLoc, nInventoryRecordId);
			const auto original_result = original(&original_pInventory, &original_pItem, nBodyLoc, nInventoryRecordId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90910 (#10279)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_RemoveInventoryItems, dll_base + 0x00050910);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			sut(&moo_pInventory);
			original(&original_pInventory);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90940 (#10280)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetTradeInventory, dll_base + 0x00050940);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90960 (#10281)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_FreeTradeInventory, dll_base + 0x00050960);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			sut(&moo_pInventory);
			original(&original_pInventory);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD909B0 (#10282)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CheckForItemInTradeInventory, dll_base + 0x000509B0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			int nItemId{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nItemId);
			const auto original_result = original(&original_pInventory, nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD909F0 (#10283)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_AddItemToTradeInventory, dll_base + 0x000509F0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			sut(&moo_pInventory, &moo_pItem);
			original(&original_pInventory, &original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90AB0 (#10316)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10316, dll_base + 0x00050AB0);
		
		SUBCASE("")
		{
			// Input data
			D2CorpseStrc moo_pCorpse{};
			D2CorpseStrc original_pCorpse{};

			const auto setup_data = [](
				D2CorpseStrc& pCorpse
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCorpse);
			setup_data(original_pCorpse);

			// Call both implementations
			const auto moo_result = sut(&moo_pCorpse);
			const auto original_result = original(&original_pCorpse);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCorpse, original_pCorpse, "Comparing pCorpse");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90AC0 (#10284)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemCount, dll_base + 0x00050AC0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90AE0 (#10285)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetBackPackItemByType, dll_base + 0x00050AE0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pCheckItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pCheckItem{};
			int nItemType{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pCheckItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pCheckItem);
			setup_data(original_pInventory, original_pCheckItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nItemType, &moo_pCheckItem);
			const auto original_result = original(&original_pInventory, nItemType, &original_pCheckItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pCheckItem, original_pCheckItem, "Comparing pCheckItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90BC0 (#10286)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetEquippedItemByType, dll_base + 0x00050BC0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pCheckItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pCheckItem{};
			int nItemType{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pCheckItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pCheckItem);
			setup_data(original_pInventory, original_pCheckItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nItemType, &moo_pCheckItem);
			const auto original_result = original(&original_pInventory, nItemType, &original_pCheckItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pCheckItem, original_pCheckItem, "Comparing pCheckItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90C80 (#10287)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetEquippedItemByCode, dll_base + 0x00050C80);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pCheckItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pCheckItem{};
			int nItemCode{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pCheckItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pCheckItem);
			setup_data(original_pInventory, original_pCheckItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nItemCode, &moo_pCheckItem);
			const auto original_result = original(&original_pInventory, nItemCode, &original_pCheckItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pCheckItem, original_pCheckItem, "Comparing pCheckItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90D50 (#11306)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetBackPackItemByCode, dll_base + 0x00050D50);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pCheckItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pCheckItem{};
			int nItemCode{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pCheckItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pCheckItem);
			setup_data(original_pInventory, original_pCheckItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nItemCode, &moo_pCheckItem);
			const auto original_result = original(&original_pInventory, nItemCode, &original_pCheckItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pCheckItem, original_pCheckItem, "Comparing pCheckItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90E20 (#10288)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetSetItemEquipCountByFileIndex, dll_base + 0x00050E20);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			int nItemFileIndex{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nItemFileIndex);
			const auto original_result = original(&original_pInventory, nItemFileIndex);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90ED0 (#10289)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_UpdateWeaponGUIDOnInsert, dll_base + 0x00050ED0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			sut(&moo_pInventory, &moo_pItem);
			original(&original_pInventory, &original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90F80 (#10290)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_UpdateWeaponGUIDOnRemoval, dll_base + 0x00050F80);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			sut(&moo_pInventory, &moo_pItem);
			original(&original_pInventory, &original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD91050 (#10291)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetWieldType, dll_base + 0x00051050);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc original_pPlayer{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer,
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer, moo_pInventory);
			setup_data(original_pPlayer, original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pPlayer, &moo_pInventory);
			const auto original_result = original(&original_pPlayer, &original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD91140 (#10292)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_SetOwnerId, dll_base + 0x00051140);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			D2UnitGUID nOwnerGuid{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			sut(&moo_pInventory, nOwnerGuid);
			original(&original_pInventory, nOwnerGuid);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD91160 (#10293)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetOwnerId, dll_base + 0x00051160);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD91190 (#10294)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CreateCorpseForPlayer, dll_base + 0x00051190);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			int nUnitId{};
			int a3{};
			int a4{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			sut(&moo_pInventory, nUnitId, a3, a4);
			original(&original_pInventory, nUnitId, a3, a4);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD91210 (#10295)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_FreeCorpse, dll_base + 0x00051210);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			int nUnitId{};
			int a3{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nUnitId, a3);
			const auto original_result = original(&original_pInventory, nUnitId, a3);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD91290 (#10296)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetFirstCorpse, dll_base + 0x00051290);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD912B0 (#10297)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetCorpseCount, dll_base + 0x000512B0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD912D0 (#10313)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetNextCorpse, dll_base + 0x000512D0);
		
		SUBCASE("")
		{
			// Input data
			D2CorpseStrc moo_pCorpse{};
			D2CorpseStrc original_pCorpse{};

			const auto setup_data = [](
				D2CorpseStrc& pCorpse
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCorpse);
			setup_data(original_pCorpse);

			// Call both implementations
			const auto moo_result = sut(&moo_pCorpse);
			const auto original_result = original(&original_pCorpse);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCorpse, original_pCorpse, "Comparing pCorpse");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFEA0 (#10314)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetUnitGUIDFromCorpse, dll_base + 0x0006FEA0);
		
		SUBCASE("")
		{
			// Input data
			D2CorpseStrc moo_pCorpse{};
			D2CorpseStrc original_pCorpse{};

			const auto setup_data = [](
				D2CorpseStrc& pCorpse
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCorpse);
			setup_data(original_pCorpse);

			// Call both implementations
			const auto moo_result = sut(&moo_pCorpse);
			const auto original_result = original(&original_pCorpse);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCorpse, original_pCorpse, "Comparing pCorpse");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB18D0 (#10315)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10315, dll_base + 0x000718D0);
		
		SUBCASE("")
		{
			// Input data
			D2CorpseStrc moo_pCorpse{};
			D2CorpseStrc original_pCorpse{};

			const auto setup_data = [](
				D2CorpseStrc& pCorpse
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCorpse);
			setup_data(original_pCorpse);

			// Call both implementations
			const auto moo_result = sut(&moo_pCorpse);
			const auto original_result = original(&original_pCorpse);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCorpse, original_pCorpse, "Comparing pCorpse");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD912F0 (#10298)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemSaveGfxInfo, dll_base + 0x000512F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			uint8_t moo_pComponents{};
			uint8_t moo_pColor{};
			D2UnitStrc original_pPlayer{};
			uint8_t original_pComponents{};
			uint8_t original_pColor{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer,
				uint8_t& pComponents,
				uint8_t& pColor
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer, moo_pComponents, moo_pColor);
			setup_data(original_pPlayer, original_pComponents, original_pColor);

			// Call both implementations
			sut(&moo_pPlayer, &moo_pComponents, &moo_pColor);
			original(&original_pPlayer, &original_pComponents, &original_pColor);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
			SKIP_MOO_CHECK_EQ(moo_pComponents, original_pComponents, "Comparing pComponents");
			SKIP_MOO_CHECK_EQ(moo_pColor, original_pColor, "Comparing pColor");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD915C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_InitializeComponentArray, dll_base + 0x000515C0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD917B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD917B0, dll_base + 0x000517B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			uint8_t moo_a2{};
			uint8_t moo_pColor{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pUnit{};
			uint8_t original_a2{};
			uint8_t original_pColor{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				uint8_t& a2,
				uint8_t& pColor,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_a2, moo_pColor, moo_pItem);
			setup_data(original_pUnit, original_a2, original_pColor, original_pItem);

			// Call both implementations
			sut(&moo_pUnit, &moo_a2, &moo_pColor, &moo_pItem);
			original(&original_pUnit, &original_a2, &original_pColor, &original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_a2, original_a2, "Comparing a2");
			SKIP_MOO_CHECK_EQ(moo_pColor, original_pColor, "Comparing pColor");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD91B60 (#10299)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10299, dll_base + 0x00051B60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			int nBodyLoc{};
			BOOL bDontCheckReqs{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem);
			setup_data(original_pUnit, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nBodyLoc, &moo_pItem, bDontCheckReqs);
			const auto original_result = original(&original_pUnit, nBodyLoc, &original_pItem, bDontCheckReqs);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD91D50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD91D50, dll_base + 0x00051D50);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc moo_a3{};
			D2UnitStrc moo_a4{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pPlayer{};
			D2UnitStrc original_a3{};
			D2UnitStrc original_a4{};
			D2UnitStrc original_pItem{};
			int a2{};
			int nBodyLoc{};
			int nUnused{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer,
				D2UnitStrc& a3,
				D2UnitStrc& a4,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer, moo_a3, moo_a4, moo_pItem);
			setup_data(original_pPlayer, original_a3, original_a4, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pPlayer, a2, nBodyLoc, &moo_a3, &moo_a4, &moo_pItem, nUnused);
			const auto original_result = original(&original_pPlayer, a2, nBodyLoc, &original_a3, &original_a4, &original_pItem, nUnused);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
			SKIP_MOO_CHECK_EQ(moo_a4, original_a4, "Comparing a4");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD91E80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD91E80, dll_base + 0x00051E80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem1{};
			D2UnitStrc moo_pItem2{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem1{};
			D2UnitStrc original_pItem2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem1,
				D2UnitStrc& pItem2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem1, moo_pItem2);
			setup_data(original_pUnit, original_pItem1, original_pItem2);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pItem1, &moo_pItem2);
			const auto original_result = original(&original_pUnit, &original_pItem1, &original_pItem2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem1, original_pItem1, "Comparing pItem1");
			SKIP_MOO_CHECK_EQ(moo_pItem2, original_pItem2, "Comparing pItem2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92080 (#10304)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetNextItem, dll_base + 0x00052080);
		
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
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD920C0 (#10305)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_UnitIsItem, dll_base + 0x000520C0);
		
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
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD920E0 (#10306)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemGUID, dll_base + 0x000520E0);
		
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
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92100 (#10307)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemNodePage, dll_base + 0x00052100);
		
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
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92140 (#10310)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_IsItemInInventory, dll_base + 0x00052140);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem);
			const auto original_result = original(&original_pInventory, &original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFEA0 (#10311)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetNextNode, dll_base + 0x0006FEA0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryNodeStrc moo_pNode{};
			D2InventoryNodeStrc original_pNode{};

			const auto setup_data = [](
				D2InventoryNodeStrc& pNode
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pNode);
			setup_data(original_pNode);

			// Call both implementations
			const auto moo_result = sut(&moo_pNode);
			const auto original_result = original(&original_pNode);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pNode, original_pNode, "Comparing pNode");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD90AB0 (#10312)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_GetItemGUIDFromNode, dll_base + 0x00050AB0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryNodeStrc moo_pNode{};
			D2InventoryNodeStrc original_pNode{};

			const auto setup_data = [](
				D2InventoryNodeStrc& pNode
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pNode);
			setup_data(original_pNode);

			// Call both implementations
			const auto moo_result = sut(&moo_pNode);
			const auto original_result = original(&original_pNode);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pNode, original_pNode, "Comparing pNode");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92180 (#10300)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_RemoveAllItems, dll_base + 0x00052180);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory);
			const auto original_result = original(&original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD921D0 (#10302)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CanItemsBeTraded, dll_base + 0x000521D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer1{};
			D2UnitStrc moo_pPlayer2{};
			D2TradeStates moo_pTradeState{};
			D2UnitStrc original_pPlayer1{};
			D2UnitStrc original_pPlayer2{};
			D2TradeStates original_pTradeState{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2UnitStrc& pPlayer1,
				D2UnitStrc& pPlayer2,
				D2TradeStates& pTradeState
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer1, moo_pPlayer2, moo_pTradeState);
			setup_data(original_pPlayer1, original_pPlayer2, original_pTradeState);

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool, &moo_pPlayer1, &moo_pPlayer2, &moo_pTradeState);
			const auto original_result = original(original_pMemPool, &original_pPlayer1, &original_pPlayer2, &original_pTradeState);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pPlayer1, original_pPlayer1, "Comparing pPlayer1");
			SKIP_MOO_CHECK_EQ(moo_pPlayer2, original_pPlayer2, "Comparing pPlayer2");
			SKIP_MOO_CHECK_EQ(moo_pTradeState, original_pTradeState, "Comparing pTradeState");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD923C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CopyUnitItemsToTradeInventory, dll_base + 0x000523C0);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pTradeInventory{};
			D2UnitStrc moo_pUnit{};
			D2InventoryStrc original_pTradeInventory{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2InventoryStrc& pTradeInventory,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTradeInventory, moo_pUnit);
			setup_data(original_pTradeInventory, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pTradeInventory, &moo_pUnit);
			const auto original_result = original(&original_pTradeInventory, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTradeInventory, original_pTradeInventory, "Comparing pTradeInventory");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92490" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(INVENTORY_CanItemBePlacedInInventory, dll_base + 0x00052490);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc original_pPlayer{};
			D2UnitStrc original_pItem{};
			D2InventoryStrc original_pInventory{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer,
				D2UnitStrc& pItem,
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer, moo_pItem, moo_pInventory);
			setup_data(original_pPlayer, original_pItem, original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pPlayer, &moo_pItem, &moo_pInventory);
			const auto original_result = original(&original_pPlayer, &original_pItem, &original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD925E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetXPosition, dll_base + 0x000525E0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD92610" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetYPosition, dll_base + 0x00052610);
		
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
}
#endif
