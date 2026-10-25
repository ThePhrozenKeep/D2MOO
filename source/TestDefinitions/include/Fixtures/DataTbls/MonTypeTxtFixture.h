#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonTypeTxtFixture : Fixture
{
	std::unique_ptr<D2MonTypeTxt[]> montype_txt;
	int montype_record_count;

	MonTypeTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, montype] = read_bin_file<D2MonTypeTxt>(working_directory / "data/global/excel" / "MonType.bin");

		sgptDataTables->nMonTypeTxtRecordCount = record_count;
		sgptDataTables->pMonTypeTxt = montype.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_montype = reinterpret_cast<D2MonTypeTxt**>(d2common_base + 0x000A9608 + 0x00000C3C);
		*original_montype = montype.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000C40);
		*original_record_count = record_count;

		montype_txt = std::move(montype);
		montype_record_count = record_count;
	};
};
