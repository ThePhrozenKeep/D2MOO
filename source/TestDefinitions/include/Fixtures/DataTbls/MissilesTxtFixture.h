#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MissilesTxtFixture : Fixture
{
	std::unique_ptr<D2MissilesTxt[]> missiles_txt;
	int missiles_record_count;

	MissilesTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, missiles] = read_bin_file<D2MissilesTxt>(working_directory / "data/global/excel" / "Missiles.bin");

		sgptDataTables->nMissilesTxtRecordCount = record_count;
		sgptDataTables->pMissilesTxt = missiles.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_missiles = reinterpret_cast<D2MissilesTxt**>(d2common_base + 0x000A9608 + 0x00000B64);
		*original_missiles = missiles.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000B6C);
		*original_record_count = record_count;

		missiles_txt = std::move(missiles);
		missiles_record_count = record_count;
	};
};
