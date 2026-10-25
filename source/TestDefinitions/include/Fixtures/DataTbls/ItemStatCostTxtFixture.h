#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ItemStatCostTxtFixture : Fixture
{
	std::unique_ptr<D2ItemStatCostTxt[]> itemstatcost_txt;
	int itemstatcost_record_count;

	ItemStatCostTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, itemstatcost] = read_bin_file<D2ItemStatCostTxt>(working_directory / "data/global/excel" / "ItemStatCost.bin");

		sgptDataTables->nItemStatCostTxtRecordCount = record_count;
		sgptDataTables->pItemStatCostTxt = itemstatcost.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_itemstatcost = reinterpret_cast<D2ItemStatCostTxt**>(d2common_base + 0x000A9608 + 0x00000BCC);
		*original_itemstatcost = itemstatcost.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000BD4);
		*original_record_count = record_count;

		itemstatcost_txt = std::move(itemstatcost);
		itemstatcost_record_count = record_count;
	};
};
