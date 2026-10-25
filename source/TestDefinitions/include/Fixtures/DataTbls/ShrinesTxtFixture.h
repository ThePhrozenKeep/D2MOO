#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ShrinesTxtFixture : Fixture
{
	std::unique_ptr<D2ShrinesTxt[]> shrines_txt;
	int shrines_record_count;

	ShrinesTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, shrines] = read_bin_file<D2ShrinesTxt>(working_directory / "data/global/excel" / "Shrines.bin");

		sgptDataTables->nShrinesTxtRecordCount = record_count;
		sgptDataTables->pShrinesTxt = shrines.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_shrines = reinterpret_cast<D2ShrinesTxt**>(d2common_base + 0x000A9608 + 0x00001094);
		*original_shrines = shrines.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00001098);
		*original_record_count = record_count;

		shrines_txt = std::move(shrines);
		shrines_record_count = record_count;
	};
};
