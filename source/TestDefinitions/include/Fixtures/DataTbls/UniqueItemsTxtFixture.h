#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct UniqueItemsTxtFixture : Fixture
{
	std::unique_ptr<D2UniqueItemsTxt[]> uniqueitems_txt;
	int uniqueitems_record_count;

	UniqueItemsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, uniqueitems] = read_bin_file<D2UniqueItemsTxt>(working_directory / "data/global/excel" / "UniqueItems.bin");

		sgptDataTables->nUniqueItemsTxtRecordCount = record_count;
		sgptDataTables->pUniqueItemsTxt = uniqueitems.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_uniqueitems = reinterpret_cast<D2UniqueItemsTxt**>(d2common_base + 0x000A9608 + 0x00000C24);
		*original_uniqueitems = uniqueitems.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000C28);
		*original_record_count = record_count;

		uniqueitems_txt = std::move(uniqueitems);
		uniqueitems_record_count = record_count;
	};
};
