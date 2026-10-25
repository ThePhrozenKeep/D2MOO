#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonModeTxtFixture : Fixture
{
	std::unique_ptr<D2MonModeTxt[]> monmode_txt;
	int monmode_record_count;

	MonModeTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monmode] = read_bin_file<D2MonModeTxt>(working_directory / "data/global/excel" / "MonMode.bin");

		sgptDataTables->pMonModeDataTables.nMonModeTxtRecordCount = record_count;
		sgptDataTables->pMonModeDataTables.pMonModeTxt = monmode.get();
		sgptDataTables->pMonModeDataTables.pMonMode[0] = monmode.get();
		sgptDataTables->pMonModeDataTables.pMonMode[1] = monmode.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monmode = reinterpret_cast<D2MonModeTxt**>(d2common_base + 0x000A9608 + 0x000010B4);
		*original_monmode = monmode.get();

		const auto original_monmode1 = reinterpret_cast<D2MonModeTxt**>(d2common_base + 0x000A9608 + 0x000010B8);
		*original_monmode1 = monmode.get();

		const auto original_monmode2 = reinterpret_cast<D2MonModeTxt**>(d2common_base + 0x000A9608 + 0x000010BC);
		*original_monmode2 = monmode.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x000010B0);
		*original_record_count = record_count;

		monmode_txt = std::move(monmode);
		monmode_record_count = record_count;
	};
};
