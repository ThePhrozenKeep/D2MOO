#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct SuperUniquesTxtFixture : Fixture
{
	std::unique_ptr<D2SuperUniquesTxt[]> superuniques_txt;
	int superuniques_record_count;

	SuperUniquesTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, superuniques] = read_bin_file<D2SuperUniquesTxt>(working_directory / "data/global/excel" / "SuperUniques.bin");

		sgptDataTables->nSuperUniquesTxtRecordCount = record_count;
		sgptDataTables->pSuperUniquesTxt = superuniques.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_superuniques = reinterpret_cast<D2SuperUniquesTxt**>(d2common_base + 0x000A9608 + 0x00000AD4);
		*original_superuniques = superuniques.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000ADC);
		*original_record_count = record_count;

		superuniques_txt = std::move(superuniques);
		superuniques_record_count = record_count;
	};
};
