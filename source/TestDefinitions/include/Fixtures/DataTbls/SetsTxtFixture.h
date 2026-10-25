#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct SetsTxtFixture : Fixture
{
	std::unique_ptr<D2SetsTxt[]> sets_txt;
	int sets_record_count;

	SetsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, sets] = read_bin_file<D2SetsTxt>(working_directory / "data/global/excel" / "Sets.bin");

		sgptDataTables->nSetsTxtRecordCount = record_count;
		sgptDataTables->pSetsTxt = sets.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_sets = reinterpret_cast<D2SetsTxt**>(d2common_base + 0x000A9608 + 0x00000C0C);
		*original_sets = sets.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000C10);
		*original_record_count = record_count;

		sets_txt = std::move(sets);
		sets_record_count = record_count;
	};
};
