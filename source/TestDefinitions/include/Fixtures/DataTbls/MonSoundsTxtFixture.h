#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonSoundsTxtFixture : Fixture
{
	std::unique_ptr<D2MonSoundsTxt[]> monsounds_txt;
	int monsounds_record_count;

	MonSoundsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monsounds] = read_bin_file<D2MonSoundsTxt>(working_directory / "data/global/excel" / "MonSounds.bin");

		sgptDataTables->nMonSoundsTxtRecordCount = record_count;
		sgptDataTables->pMonSoundsTxt = monsounds.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monsounds = reinterpret_cast<D2MonSoundsTxt**>(d2common_base + 0x000A9608 + 0x00000A84);
		*original_monsounds = monsounds.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000A8C);
		*original_record_count = record_count;

		monsounds_txt = std::move(monsounds);
		monsounds_record_count = record_count;
	};
};
