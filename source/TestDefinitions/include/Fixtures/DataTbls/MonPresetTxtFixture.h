#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonPresetTxtFixture : Fixture
{
	std::unique_ptr<D2MonPresetTxt[]> monpreset_txt;
	int monpreset_record_count;

	MonPresetTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monpreset] = read_bin_file<D2MonPresetTxt>(working_directory / "data/global/excel" / "MonPreset.bin");

		sgptDataTables->pMonPresetTxt = monpreset.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monpreset = reinterpret_cast<D2MonPresetTxt**>(d2common_base + 0x000A9608 + 0x00000AA8);
		*original_monpreset = monpreset.get();

		monpreset_txt = std::move(monpreset);
		monpreset_record_count = record_count;
	};
};
