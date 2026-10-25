#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct LvlPrestTxtFixture : Fixture
{
	std::unique_ptr<D2LvlPrestTxt[]> lvlprest_txt;
	int lvlprest_record_count;

	LvlPrestTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, lvlprest] = read_bin_file<D2LvlPrestTxt>(working_directory / "data/global/excel" / "LvlPrest.bin");

		sgptDataTables->nLvlPrestTxtRecordCount = record_count;
		sgptDataTables->pLvlPrestTxt = lvlprest.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_lvlprest = reinterpret_cast<D2LvlPrestTxt**>(d2common_base + 0x000A9608 + 0x00000C64);
		*original_lvlprest = lvlprest.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000C68);
		*original_record_count = record_count;

		lvlprest_txt = std::move(lvlprest);
		lvlprest_record_count = record_count;
	};
};
