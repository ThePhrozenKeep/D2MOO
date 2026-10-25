#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct CharStatsTxtFixture : Fixture
{
	std::unique_ptr<D2CharStatsTxt[]> charstats_txt;
	int charstats_record_count;

	CharStatsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, charstats] = read_bin_file<D2CharStatsTxt>(working_directory / "data/global/excel" / "CharStats.bin");

		sgptDataTables->nCharStatsTxtRecordCount = record_count;
		sgptDataTables->pCharStatsTxt = charstats.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_charstats = reinterpret_cast<D2CharStatsTxt**>(d2common_base + 0x000A9608 + 0x00000BC4);
		*original_charstats = charstats.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000BC8);
		*original_record_count = record_count;

		charstats_txt = std::move(charstats);
		charstats_record_count = record_count;
	};
};
