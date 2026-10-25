#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct LvlWarpTxtFixture : Fixture
{
	std::unique_ptr<D2LvlWarpTxt[]> lvlwarp_txt;
	int lvlwarp_record_count;

	LvlWarpTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, lvlwarp] = read_bin_file<D2LvlWarpTxt>(working_directory / "data/global/excel" / "LvlWarp.bin");

		sgptDataTables->nLvlWarpTxtRecordCount = record_count;
		sgptDataTables->pLvlWarpTxt = lvlwarp.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_lvlwarp = reinterpret_cast<D2LvlWarpTxt**>(d2common_base + 0x000A9608 + 0x00001028);
		*original_lvlwarp = lvlwarp.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x0000102C);
		*original_record_count = record_count;

		lvlwarp_txt = std::move(lvlwarp);
		lvlwarp_record_count = record_count;
	};
};
