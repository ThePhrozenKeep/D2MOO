#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonPlaceTxtFixture : Fixture
{
	std::unique_ptr<D2MonPlaceTxt[]> monplace_txt;
	int monplace_record_count;

	MonPlaceTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monplace] = read_bin_file<D2MonPlaceTxt>(working_directory / "data/global/excel" / "MonPlace.bin");

		sgptDataTables->nMonPlaceTxtRecordCount = record_count;
		sgptDataTables->pMonPlaceTxt = monplace.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monplace = reinterpret_cast<D2MonPlaceTxt**>(d2common_base + 0x000A9608 + 0x00000A9C);
		*original_monplace = monplace.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000AA4);
		*original_record_count = record_count;

		monplace_txt = std::move(monplace);
		monplace_record_count = record_count;
	};
};
