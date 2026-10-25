#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonUModTxtFixture : Fixture
{
	std::unique_ptr<D2MonUModTxt[]> monumod_txt;
	int monumod_record_count;

	MonUModTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monumod] = read_bin_file<D2MonUModTxt>(working_directory / "data/global/excel" / "MonUMod.bin");

		sgptDataTables->nMonUModTxtRecordCount = record_count;
		sgptDataTables->pMonUModTxt = monumod.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monumod = reinterpret_cast<D2MonUModTxt**>(d2common_base + 0x000A9608 + 0x00000C50);
		*original_monumod = monumod.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000C54);
		*original_record_count = record_count;

		monumod_txt = std::move(monumod);
		monumod_record_count = record_count;
	};
};
