#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct LvlTypesTxtFixture : Fixture
{
	std::unique_ptr<D2LvlTypesTxt[]> lvltypes_txt;
	int lvltypes_record_count;

	LvlTypesTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, lvltypes] = read_bin_file<D2LvlTypesTxt>(working_directory / "data/global/excel" / "LvlTypes.bin");

		sgptDataTables->nLvlTypesTxtRecordCount = record_count;
		sgptDataTables->pLvlTypesTxt = lvltypes.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_lvltypes = reinterpret_cast<D2LvlTypesTxt**>(d2common_base + 0x000A9608 + 0x00001018);
		*original_lvltypes = lvltypes.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00001024);
		*original_record_count = record_count;

		lvltypes_txt = std::move(lvltypes);
		lvltypes_record_count = record_count;
	};
};
