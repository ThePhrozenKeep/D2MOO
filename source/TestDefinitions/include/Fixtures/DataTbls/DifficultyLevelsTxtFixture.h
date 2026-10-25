#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct DifficultyLevelsTxtFixture : Fixture
{
	std::unique_ptr<D2DifficultyLevelsTxt[]> difficultylevels_txt;
	int difficultylevels_record_count;

	DifficultyLevelsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, difficultylevels] = read_bin_file<D2DifficultyLevelsTxt>(working_directory / "data/global/excel" / "DifficultyLevels.bin");

		sgptDataTables->nDifficultyLevelsTxtRecordCount = record_count;
		sgptDataTables->pDifficultyLevelsTxt = difficultylevels.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_difficultylevels = reinterpret_cast<D2DifficultyLevelsTxt**>(d2common_base + 0x000A9608 + 0x00000C7C);
		*original_difficultylevels = difficultylevels.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000C80);
		*original_record_count = record_count;

		difficultylevels_txt = std::move(difficultylevels);
		difficultylevels_record_count = record_count;
	};
};
