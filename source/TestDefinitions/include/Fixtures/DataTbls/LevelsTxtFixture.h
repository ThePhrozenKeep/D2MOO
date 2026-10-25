#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct LevelsTxtFixture : Fixture
{
	std::unique_ptr<D2LevelsTxt[]> levels_txt;
	int levels_record_count;

	LevelsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, levels] = read_bin_file<D2LevelsTxt>(working_directory / "data/global/excel" / "Levels.bin");

		sgptDataTables->nLevelsTxtRecordCount = record_count;
		sgptDataTables->pLevelsTxt = levels.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_levels = reinterpret_cast<D2LevelsTxt**>(d2common_base + 0x000A9608 + 0x00000C58);
		*original_levels = levels.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000C5C);
		*original_record_count = record_count;

		levels_txt = std::move(levels);
		levels_record_count = record_count;
	};
};
