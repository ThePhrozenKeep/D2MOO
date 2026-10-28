#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct UniqueAppellationTxtFixture : Fixture
{
	std::unique_ptr<D2UniqueAppellationTxt[]> uniqueappellation_txt;
	int uniqueappellation_record_count;

	UniqueAppellationTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, uniqueappellation] = read_bin_file<D2UniqueAppellationTxt>(working_directory / "data/global/excel" / "UniqueAppellation.bin");

		sgptDataTables->nUniqueAppellationTxtRecordCount = record_count;
		sgptDataTables->pUniqueAppellationTxt = uniqueappellation.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_uniqueappellation = reinterpret_cast<D2UniqueAppellationTxt**>(d2common_base + 0x000A9608 + 0x00001074);
		*original_uniqueappellation = uniqueappellation.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00001084);
		*original_record_count = record_count;

		uniqueappellation_txt = std::move(uniqueappellation);
		uniqueappellation_record_count = record_count;
	};
};
