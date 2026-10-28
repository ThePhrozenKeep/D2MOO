#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MissCalcTxtFixture : Fixture
{
	std::unique_ptr<D2MissCalcTxt[]> misscalc_txt;
	int misscalc_record_count;

	MissCalcTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, misscalc] = read_bin_file<D2MissCalcTxt>(working_directory / "data/global/excel" / "MissCalc.bin");

		sgptDataTables->pMissCalcTxt = misscalc.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_misscalc = reinterpret_cast<D2MissCalcTxt**>(d2common_base + 0x000A9608 + 0x00000058);
		*original_misscalc = misscalc.get();

		misscalc_txt = std::move(misscalc);
		misscalc_record_count = record_count;
	};
};
