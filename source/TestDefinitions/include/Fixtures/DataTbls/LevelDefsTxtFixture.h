#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct LevelDefsTxtFixture : Fixture
{
	std::unique_ptr<D2LevelDefBin[]> leveldefs_txt;
	int leveldefs_record_count;

	LevelDefsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, leveldefs] = read_bin_file<D2LevelDefBin>(working_directory / "data/global/excel" / "LevelDefs.bin");

		sgptDataTables->pLevelDefBin = leveldefs.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_leveldefs = reinterpret_cast<D2LevelDefBin**>(d2common_base + 0x000A9608 + 0x00000C60);
		*original_leveldefs = leveldefs.get();

		leveldefs_txt = std::move(leveldefs);
		leveldefs_record_count = record_count;
	};
};
