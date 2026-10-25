#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct SetItemsTxtFixture : Fixture
{
	std::unique_ptr<D2SetItemsTxt[]> setitems_txt;
	int setitems_record_count;

	SetItemsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, setitems] = read_bin_file<D2SetItemsTxt>(working_directory / "data/global/excel" / "SetItems.bin");

		sgptDataTables->nSetItemsTxtRecordCount = record_count;
		sgptDataTables->pSetItemsTxt = setitems.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_setitems = reinterpret_cast<D2SetItemsTxt**>(d2common_base + 0x000A9608 + 0x00000C18);
		*original_setitems = setitems.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000C1C);
		*original_record_count = record_count;

		setitems_txt = std::move(setitems);
		setitems_record_count = record_count;
	};
};
