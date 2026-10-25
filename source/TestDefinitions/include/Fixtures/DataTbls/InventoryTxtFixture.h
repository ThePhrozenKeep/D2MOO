#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct InventoryTxtFixture : Fixture
{
	std::unique_ptr<D2InventoryTxt[]> inventory_txt;
	int inventory_record_count;

	InventoryTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, inventory] = read_bin_file<D2InventoryTxt>(working_directory / "data/global/excel" / "Inventory.bin");

		sgptDataTables->nInventoryTxtRecordCount = record_count;
		sgptDataTables->pInventoryTxt = inventory.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_inventory = reinterpret_cast<D2InventoryTxt**>(d2common_base + 0x000A9608 + 0x00000CD0);
		*original_inventory = inventory.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000CCC);
		*original_record_count = record_count;

		inventory_txt = std::move(inventory);
		inventory_record_count = record_count;
	};
};
