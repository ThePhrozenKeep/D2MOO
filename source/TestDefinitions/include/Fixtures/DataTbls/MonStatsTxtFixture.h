#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonStatsTxtFixture : Fixture
{
	std::unique_ptr<D2MonStatsTxt[]> monstats_txt;
	int monstats_record_count;

	MonStatsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monstats] = read_bin_file<D2MonStatsTxt>(working_directory / "data/global/excel" / "MonStats.bin");

		sgptDataTables->nMonStatsTxtRecordCount = record_count;
		sgptDataTables->pMonStatsTxt = monstats.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monstats = reinterpret_cast<D2MonStatsTxt**>(d2common_base + 0x000A9608 + 0x00000A78);
		*original_monstats = monstats.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000A80);
		*original_record_count = record_count;

		monstats_txt = std::move(monstats);
		monstats_record_count = record_count;
	};
};
