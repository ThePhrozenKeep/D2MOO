#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonStats2TxtFixture : Fixture
{
	std::unique_ptr<D2MonStats2Txt[]> monstats2_txt;
	int monstats2_record_count;

	MonStats2TxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monstats2] = read_bin_file<D2MonStats2Txt>(working_directory / "data/global/excel" / "MonStats2.bin");

		sgptDataTables->nMonStats2TxtRecordCount = record_count;
		sgptDataTables->pMonStats2Txt = monstats2.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monstats2 = reinterpret_cast<D2MonStats2Txt**>(d2common_base + 0x000A9608 + 0x00000A90);
		*original_monstats2 = monstats2.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000A98);
		*original_record_count = record_count;

		monstats2_txt = std::move(monstats2);
		monstats2_record_count = record_count;
	};
};
