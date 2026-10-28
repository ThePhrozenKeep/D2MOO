#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonLvlTxtFixture : Fixture
{
	std::unique_ptr<D2MonLvlTxt[]> monlvl_txt;
	int monlvl_record_count;

	MonLvlTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monlvl] = read_bin_file<D2MonLvlTxt>(working_directory / "data/global/excel" / "MonLvl.bin");

		sgptDataTables->nMonLvlTxtRecordCount = record_count;
		sgptDataTables->pMonLvlTxt = monlvl.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monlvl = reinterpret_cast<D2MonLvlTxt**>(d2common_base + 0x000A9608 + 0x00000B70);
		*original_monlvl = monlvl.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000B74);
		*original_record_count = record_count;

		monlvl_txt = std::move(monlvl);
		monlvl_record_count = record_count;
	};
};
