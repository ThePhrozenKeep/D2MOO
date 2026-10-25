#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonPropTxtFixture : Fixture
{
	std::unique_ptr<D2MonPropTxt[]> monprop_txt;
	int monprop_record_count;

	MonPropTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monprop] = read_bin_file<D2MonPropTxt>(working_directory / "data/global/excel" / "MonProp.bin");

		sgptDataTables->nMonPropTxtRecordCount = record_count;
		sgptDataTables->pMonPropTxt = monprop.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monprop = reinterpret_cast<D2MonPropTxt**>(d2common_base + 0x000A9608 + 0x00000C30);
		*original_monprop = monprop.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000C34);
		*original_record_count = record_count;

		monprop_txt = std::move(monprop);
		monprop_record_count = record_count;
	};
};
