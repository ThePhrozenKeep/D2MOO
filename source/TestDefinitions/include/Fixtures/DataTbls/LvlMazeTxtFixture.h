#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct LvlMazeTxtFixture : Fixture
{
	std::unique_ptr<D2LvlMazeTxt[]> lvlmaze_txt;
	int lvlmaze_record_count;

	LvlMazeTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, lvlmaze] = read_bin_file<D2LvlMazeTxt>(working_directory / "data/global/excel" / "LvlMaze.bin");

		sgptDataTables->nLvlMazeTxtRecordCount = record_count;
		sgptDataTables->pLvlMazeTxt = lvlmaze.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_lvlmaze = reinterpret_cast<D2LvlMazeTxt**>(d2common_base + 0x000A9608 + 0x00001030);
		*original_lvlmaze = lvlmaze.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00001034);
		*original_record_count = record_count;

		lvlmaze_txt = std::move(lvlmaze);
		lvlmaze_record_count = record_count;
	};
};
